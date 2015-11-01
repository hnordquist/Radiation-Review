// VifmImport.cpp: implementation of the CVifmImport class.
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	Copyright, 2002, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to wrap all calls to the VIFMCom object.
//
//	Functions:
//		LocalCoInitialize	
//		LocalCoUninitialize
//		OpenVifmImportDatabase
//		VifmImport::CVifmImport
//		CVifmImport::~CVifmImport
//		CVifmImport::OpenDatabase
//		CVifmImport::InitializeDatabase
//		CVifmImport::CloseDatabase
//		CVifmImport::AddData
//		CVifmImport::GetDbStartDay
//		CVifmImport::GetDbEndDay
//		CVifmImport::GetDataStatistics
//		CVifmImport::GetDataByChannel
//		CVifmImport::GetDataByStation
//		cVifmImport::GetDayStatistics		
//		CVifmImport::GetDataByDay
///////////////////////////////////////////////////////////////////////////
#include "RadBuild.h"
#include "ComDataStructures.h"
#include "VifmImport.h"
#include "imports.h"
#include "MyDateTime.h"
#include "SafeArrayUtil.h"
#include "TimeAlign.h"
#include "TypeDefinitions.h"

IVifmImportPtr pIVifmImport;
///////////////////////////////////////////////////////////////////////////
//	Name:	OpenVifmImportDatabase
//
//	Description:
//	Creates the VifmImportCom object and opens the database in the com object.
//	
//	Declaration:
//	int OpenVifmImportDatabase(const char* strDataStorePath, const char* strDefaultPath)
//
//	Input:	strDataStorePath	path where database to be used by VifmCom is located
//			strDefaultPath		path where any default parameters to be used by VifmCom are located
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
int OpenVifmImportDatabase(const short sFacilityID)
{
	if (!pglVifmImport->OpenDatabase(sFacilityID)) // couldn't open db
		return(0);
	else
		return (1);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVifmImport::CVifmImport(const char* szDefaultPath)
{
	mstrDataStorePath[0] = '\0';
	mstrDefaultPath[0] = '\0';

	strncpy(mstrDefaultPath, szDefaultPath, 256);
	pIVifmImport.CreateInstance(CLSID_VifmImport);

	if (pIVifmImport == NULL)
	{
		AfxMessageBox(
			"Could not Create Instance of VIFM Import component.\npIVifmImport = NULL.");  
	}
}

CVifmImport::~CVifmImport()
{
	pIVifmImport = NULL;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	OpenDatabase
//
//	Description:
//	Set parameters necessary in VifmCom and opens the database to be used by VifmCom.
//	
//	Declaration:
//	int CVifmImport::OpenDatabase(const char* strDataStorePath, const char* strDefaultPath)
//
//	Input:	strDataStorePath	path where database to be used by VifmCom is located
//			strDefaultPath		path where any default parameters to be used by VifmCom are located
//
//	Output:	none
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	26-Oct-2004	SFK		Removed calls to SetDataStore and SetDefaultPath to conform to Import interface.
//						Also send in FacilityID rather than path
//////////////////////////////////////////////////////////////////
bool CVifmImport::OpenDatabase(const short sFacilityID)
{
	VARIANT_BOOL vb = VARIANT_FALSE;

	if (pIVifmImport) 
		vb = pIVifmImport->OpenDataStore(sFacilityID);

	return(vb != VARIANT_FALSE);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	InitializeDatabase
//
//	Description:
//	Wraps VifmCom InitializeDatabase method.
//	
//	Declaration:
//	int CVifmImport::InitializeDatabase(const char* strDatabasePath)
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
int CVifmImport::InitializeDatabase(const short sFacilityID, const bool bQuestion)
{
	int iStatus;
	
	if (pIVifmImport) iStatus = pIVifmImport->InitializeDataStore(sFacilityID, (VARIANT_BOOL)bQuestion);

	return(iStatus);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	CloseDatabase
//
//	Description:
//	Wraps VifmCom CloseDataStore method.
//	
//	Declaration:
//	void CVifmImport::CloseDatabase()
//
//	Input:	none
//
//	Output:	none
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//////////////////////////////////////////////////////////////////
void CVifmImport::CloseDatabase()
{
	if (pIVifmImport) pIVifmImport->CloseDataStore();
}


///////////////////////////////////////////////////////////////////////////
//	Name:	AddData
//
//	Description:
//	Wraps VifmCom AddData method.
//	
//	Declaration:
//	int CVifmImport::AddData(const char* strFilename, char* strResult)
//
//	Input:	strFilename		name of file to be added to VifmCom database
//			strResult		results of file import processing -- caller must deallocate!
//
//	Output:	none
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//////////////////////////////////////////////////////////////////
int CVifmImport::AddData(const short sFacID, const char* strFilename, BOOL bOverwrite, char** strResult)
{
	int iStatus;
	BSTR temp1;
	USES_CONVERSION;
	
	if (pIVifmImport) iStatus = pIVifmImport->AddData(sFacID, strFilename, bOverwrite, &temp1);
	int len = strlen(W2T(temp1));	// determine length of string 
	*strResult = new char[len+1];	// allocate storage
	strcpy(*strResult, W2T(temp1));	// copy to storage -- caller must deallocate

	return(iStatus);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetDbStartDay
//
//	Description:
//	Wraps VifmCom GetDataStoreTimeRange method and extracts the start day in the db.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CVifmImport::GetDbStartDay(const short sFacID, const short sStaID, DATE *pdStartDay, const bool bErrDisplay)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			bErrDisplay	whether to display message if no data found in db
//
//	Output:	pdStartDay	DATE of first day in the db
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	30-Apr-2002 SFK		Received DATE data from COMs
//	13-Aug-2002	SFK		Return DATE to rest of Rad Review
//////////////////////////////////////////////////////////////////
int CVifmImport::GetDbStartDay(const short sFacID, const short sStaID, DATE *pdStartDay, const bool bErrDisplay)
{
	DATE dDumDate;

	*pdStartDay = 0.0;

	VARIANT_BOOL bStatus = false;
	if (pIVifmImport) bStatus = pIVifmImport->GetDataStoreTimeRange(sFacID, sStaID, bErrDisplay, pdStartDay, &dDumDate);
	
	return(bStatus);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetDbEndDay
//
//	Description:
//	Wraps VifmCom GetDbEndDay method and extracts the end day in the db.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CVifmImport::GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			bErrDisplay	whether to display message if no data found in db
//
//	Output:	pdEndDay	DATE of last day in the db
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	30-Apr-2002 SFK		Received DATE data from COMs
//	13-Aug-2002	SFK		Return DATE to rest of Rad Review
//////////////////////////////////////////////////////////////////
int CVifmImport::GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay)
{

	DATE dDumDate;

	*pdEndDay = 0.0;

	VARIANT_BOOL bStatus = false;
	if (pIVifmImport) bStatus = pIVifmImport->GetDataStoreTimeRange(sFacID,sStaID, bErrDisplay, &dDumDate, pdEndDay);

	return(bStatus);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataStatistics
//
//	Description:
//	Wraps VifmCom GetDataStatistics method.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CVifmImport::GetDataStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdStart, DATE *pdEnd, unsigned long *pulNumPts)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			dStart		DATE for start of search
//			dEnd		DATE for end of search
//
//	Output:	pdActualStart	DATE of first data point in the interval
//			pdActualEnd		DATE of last data point in the interval
//			pulNumPts	number of data points between and including pdStart to pdEnd
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	30-Apr-2002 SFK		Received DATE data from COMs
//	13-Aug-2002	SFK		Return DATE to rest of Rad Review
//////////////////////////////////////////////////////////////////
int CVifmImport::GetDataStatistics(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts)
{
	long lNumPts;

	*pdActualStart = 0;
	*pdActualEnd = 0;
	*pulNumPts = 0;

	VARIANT_BOOL bStatus = false;
	if (pIVifmImport) bStatus = pIVifmImport->GetDataStatistics(sFacID, sStaID, sChanID, dStart, dEnd, pdActualStart, pdActualEnd, &lNumPts);
	
	//	convert timestamps back to unsigned longs for now
	if (bStatus) { //extract info
		*pulNumPts = lNumPts;
	}

	return(bStatus);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByChannel
//
//	Description:
//	Wraps VifmCom GetDataByChannel method.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CVifmImport::GetDataByChannel(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, double *pdX, double *pdY, unsigned long *pulNumPts, DATE *pdNextTime)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			sChanID		Channel number at station (must be 0-6)
//			dStart		DATE for start of search
//			dEnd		DATE for end of search
//
//	Output:	pdX			array containing DATEs of all data in interval including endpoints
//			pdY			array containing data values of all data in interval including endpoints
//			pulNumPts	number of entries in the pdX or pdY array
//			pdNextTime	DATE of the next possible data point following the endpoint
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	30-Apr-2002 SFK		Received DATE data from COMs
//	13-Aug-2002	SFK		Return DATE to rest of Rad Review
//	28-Oct-2004	SFK		Changed to receive all data in the SAFEARRAY and parse out timestamps, data and next time
//////////////////////////////////////////////////////////////////
int CVifmImport::GetDataByChannel(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, double *pdX, double *pdY, unsigned long *pulNumPts, DATE *pdNextTime)
{
	VARIANT vData;
	VariantInit(&vData);

	long lIndex;

	*pdNextTime = 0.0;
	if (*pulNumPts == 0) return(1);

	vData.lVal = *pulNumPts;
	VARIANT_BOOL bStatus = false;
	if (pIVifmImport) bStatus = pIVifmImport->GetDataByChannel(sFacID, sStaID, sChanID, dStart, dEnd, &vData);

	// Extract data from the SAFEARRAY and return to Rad as in previous calls
	if ((bStatus && (vData.vt == (VT_VARIANT | VT_ARRAY)))) {
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vData.parray, 1, &lUBound);	
		if (lUBound > *pulNumPts) {		// got back more points than expected only return first *pulNumPts
			lUBound = *pulNumPts - 1;
		}
		for (lIndex=0; lIndex<lUBound; lIndex++) {
			SA_GetElem(vData.parray, lIndex, 0, &(pdX[lIndex]));		// timestamp in column 1
			SA_GetElem(vData.parray, lIndex, 1, &(pdY[lIndex]));		// data in column 2
		}

		*pulNumPts = lIndex;									// number of points returned
		SA_GetElem(vData.parray, lIndex, 0, pdNextTime);		// timestamp of next data point
		//SafeArrayDestroy(vData.parray);
	}

	VariantClear(&vData);
	return(bStatus);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByStation
//
//	Description:
//	Wraps VifmCom GetDataByStation method
//	The second instance of the function returns data in a double array -- to support most
//	of the analysis routines.
//	
//	Declaration:
//	int CVifmImport::GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_FLOAT_DATA_PT1 *pfData, unsigned long *pulNumPts, DATE *pdNextTime)
//	int CVifmImport::GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_DOUBLE_DATA_PT1 *pdData, unsigned long *pulNumPts, DATE *pdNextTime)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			dStart		DATE of interval start to retrieve Vifm data records
//			dEnd		DATE of interval end to retrieve Vifm data records
//			pulNumPts	number of data points expected in the range (know from GetDataStatistics)
//			pfData		user must allocate storage for pulNumPts data records before this call
//
//	Output:	pulNumPts	actual number of data points found in the interval
//			pfData		pulNumPts of records filled in the array
//			pdNextTime	DATE of the next possible data record following this interval
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	30-Apr-2002 SFK		Received DATE data from COMs
//	06-Jun-2002	SFK		Received DATE from Rad Review
//	13-Aug-2002	SFK		Return DATE to rest of Rad Review
//	24-Mar-2003 SFK		Added second function to return double data for analysis
//	28-Oct-2004	SFK		Changed to receive all data in the SAFEARRAY
//////////////////////////////////////////////////////////////////
// Get station data as floats
int CVifmImport::GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_FLOAT_DATA_PT1 *pfData, unsigned long *pulNumPts, DATE *pdNextTime)
{
	VARIANT vData;
	VariantInit(&vData);
	int iStatus = 0;
	long lIndex;

	// set default returns and check if any points requested.
	*pdNextTime = 0.0;
	if (*pulNumPts == 0) return(1);

	vData.lVal = *pulNumPts;
	iStatus = pglTimeAlign->GetDataByStation(sFacID, sStaID, dStart, dEnd, &vData, BOT_TYPE, pulNumPts);

	if (iStatus && (vData.vt == (VT_VARIANT | VT_ARRAY))) {	// extract the data
		// now pull the data back out
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vData.parray, 1, &lUBound);
		
		if (lUBound > *pulNumPts)		// got back more points than expected only return first *pulNumPts
			lUBound = *pulNumPts - 1;

		for (lIndex=0; lIndex<lUBound; lIndex++) 
		{
			SA_GetElem(vData.parray, lIndex, 0, &(pfData[lIndex].dJulianTime));
			SA_GetElem(vData.parray, lIndex, 1, (long *)&(pfData[lIndex].ulStatus));
			SA_GetElem(vData.parray, lIndex, 2, &(pfData[lIndex].fData[0]));
			SA_GetElem(vData.parray, lIndex, 3, &(pfData[lIndex].fData[1]));
			SA_GetElem(vData.parray, lIndex, 4, &(pfData[lIndex].fData[2]));
			SA_GetElem(vData.parray, lIndex, 5, &(pfData[lIndex].fData[3]));
			SA_GetElem(vData.parray, lIndex, 6, &(pfData[lIndex].fData[4]));
			SA_GetElem(vData.parray, lIndex, 7, &(pfData[lIndex].fData[5]));
			SA_GetElem(vData.parray, lIndex, 8, &(pfData[lIndex].fData[6]));
			SA_GetElem(vData.parray, lIndex, 9, &(pfData[lIndex].fData[7]));
			SA_GetElem(vData.parray, lIndex, 10, &(pfData[lIndex].dElapsedTime));
		}
		SA_GetElem(vData.parray, lIndex, 0, pdNextTime);		// timestamp of next data point
		//SafeArrayDestroy(vData.parray);
	}
	VariantClear(&vData);
	return(iStatus);
}


// put the data in the doubles for analysis
int CVifmImport::GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_DOUBLE_DATA_PT1 *pdData, unsigned long *pulNumPts, DATE *pdNextTime)
{
	VARIANT vData;
	VariantInit(&vData);
	int iStatus = 0;
	long lIndex;

	// set default returns and check if any points requested.
	*pdNextTime = 0.0;
	if (*pulNumPts == 0) return(1);

	vData.lVal = *pulNumPts;
	iStatus = pglTimeAlign->GetDataByStation(sFacID, sStaID, dStart, dEnd, &vData, BOT_TYPE, pulNumPts);

	if (iStatus && (vData.vt == (VT_VARIANT | VT_ARRAY))) {	// extract the data
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vData.parray, 1, &lUBound);	
		if (lUBound > *pulNumPts) {		// got back more points than expected only return first *pulNumPts
			lUBound = *pulNumPts - 1;
		}

		for (lIndex=0; lIndex<lUBound; lIndex++) {
		// now pull the data back out
			SA_GetElem(vData.parray, lIndex, 0, &(pdData[lIndex].dJulianTime));
			SA_GetElem(vData.parray, lIndex, 1, (long *)&(pdData[lIndex].ulStatus));
			SA_GetElem(vData.parray, lIndex, 2, &(pdData[lIndex].dData[0]));
			SA_GetElem(vData.parray, lIndex, 3, &(pdData[lIndex].dData[1]));
			SA_GetElem(vData.parray, lIndex, 4, &(pdData[lIndex].dData[2]));
			SA_GetElem(vData.parray, lIndex, 5, &(pdData[lIndex].dData[3]));
			SA_GetElem(vData.parray, lIndex, 6, &(pdData[lIndex].dData[4]));
			SA_GetElem(vData.parray, lIndex, 7, &(pdData[lIndex].dData[5]));
			SA_GetElem(vData.parray, lIndex, 8, &(pdData[lIndex].dData[6]));
			SA_GetElem(vData.parray, lIndex, 9, &(pdData[lIndex].dData[7]));
			SA_GetElem(vData.parray, lIndex, 10, &(pdData[lIndex].dElapsedTime));
		}
		SA_GetElem(vData.parray, lIndex, 0, pdNextTime);		// timestamp of n
		//SafeArrayDestroy(vData.parray);
	}
	VariantClear(&vData);
	return(iStatus);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDayStatistics
//
//	Description:
//	Wraps VifmCom GetDayStatistics VifmCom method.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CVifmImport::GetDayStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			dStart		DATE of interval start to retrieve Vifm data records
//			dEnd		DATE of interval end to retrieve Vifm data records
//
//	Output:	pulNumPts	actual number of data points found in the interval
//			pdActualStart	DATE of first day in interval
//			pdActualEnd		julian tie of last day in interval
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	30-Apr-2002 SFK		Received DATE data from COMs
//	13-Aug-2002	SFK		Return DATE to rest of Rad Review
//////////////////////////////////////////////////////////////////
int CVifmImport::GetDayStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts)
{
	long lNumPts;

	*pdActualStart = 0.0;
	*pdActualEnd = 0.0;
	*pulNumPts = 0;

	VARIANT_BOOL bStatus = false;
	if (pIVifmImport) bStatus = pIVifmImport->GetDayStatistics(sFacID, sStaID, dStart, dEnd, pdActualStart, pdActualEnd, &lNumPts);

	// convert timestamps back to unsigned longs for now
	if (bStatus) {
		*pulNumPts = lNumPts;
	}

	return(bStatus);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByDay
//
//	Description:
//	Wraps VifmCom GetDataByDay method.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CVifmImport::GetDataByDay(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			dStart		DATE of interval start to retrieve Vifm data records
//			dEnd		DATE of interval end to retrieve Vifm data records
//			pulNumDays	number of data points expected in the range (know from GetDayStatistics)
//			pDayStruct	user must allocate storage for pulNumPts day records before this call
//
//	Output:	pulNumDays	actual number of data points found in the interval
//			pDayStruct	pulNumPts of records filled in the array
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	30-Apr-2002 SFK		Received DATE data from COMs
//	12-Aug-2002	SFK		Return DATE to rest of Rad Review
//	28-Oct-2004	SFK		Renamed COM call to GetDaySummaryData from GetDataByDay.
//						Also get pulNumDays value from number of rows in the SAFEARRAY
//////////////////////////////////////////////////////////////////
int CVifmImport::GetDataByDay(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays)
{
	VARIANT vDayData;
	VariantInit(&vDayData);
	long lIndex;
	VARIANT_BOOL bStatus = false;

	//<<QA>> this always returns - this function is not really implemented.
	*pulNumDays = 0;
	if (*pulNumDays == 0) return(1);

	if (pIVifmImport) bStatus = pIVifmImport->GetDaySummaryData(sFacID, (short)sStaID, dStart, dEnd, &vDayData);
	
	// now pull the data back out
	if ((bStatus)  && (vDayData.vt == (VT_VARIANT | VT_ARRAY))) {	// is the type of SafeArray we are expecting 
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vDayData.parray, 1, &lUBound);	
		if (lUBound > *pulNumDays) {		// got back more points than expected only return first *pulNumPts
			lUBound = *pulNumDays - 1;
		}

		for (lIndex=0; lIndex<lUBound; lIndex++) {
			SA_GetElem(vDayData.parray, lIndex, 0, &(pDayStruct[lIndex].dFirstDataPt));
			SA_GetElem(vDayData.parray, lIndex, 0, &(pDayStruct[lIndex].dLastDataPt));
			SA_GetElem(vDayData.parray, lIndex, 0, (long *)&(pDayStruct[lIndex].ulNumPtsInDay));
		}
		*pulNumDays = lUBound;
		//SafeArrayDestroy(vDayData.parray);
	}
	VariantClear(&vDayData);
	return(bStatus);
}


// 28-Oct-2004	SFK	Removed this function
//int CVifmImport::SetDatabase(const char *strPath)
//{
//	int iStatus;
	
//	iStatus = pIVifmImport->SetDataStore(strPath);

//	return(iStatus);
//}

// 28-Oct-2004	SFK	Added sFacilityID arg to calling parms of function and COM call
int CVifmImport::BackupDatabase(const short sFacilityID, const char *strPath, const bool bQuestion, const bool bPrompt)
{
	int iStatus;
	
	if (pIVifmImport) iStatus = pIVifmImport->BackupDataStore(sFacilityID, strPath, (VARIANT_BOOL)bQuestion, (VARIANT_BOOL)bPrompt);
	return(iStatus);
}

// 28-Oct-2004	SFK	Added sFacilityID arg to calling parms of function and COM call
int CVifmImport::RestoreDatabase(const short sFacilityID, const char *strPath, const char *strDestPath, const bool bQuestion, const bool bPrompt)
{
	int iStatus;
	
	if (pIVifmImport) iStatus = pIVifmImport->RestoreDataStore(sFacilityID, strPath, (VARIANT_BOOL)bQuestion, (VARIANT_BOOL)bPrompt);
	//Change VARIANT_BOOL to bool
	return(iStatus!=VARIANT_FALSE);
}

// 28-Oct-2004	SFK	Added sFacilityID arg to calling parms of function and COM call
void CVifmImport::GetDataStoreSize(const short sFacilityID, double *pdSizeInBytes)
{
	if (pIVifmImport) pIVifmImport->GetDataStoreSize(sFacilityID, pdSizeInBytes);
}


