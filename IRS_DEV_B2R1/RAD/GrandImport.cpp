// GrandImport.cpp: implementation of the CGrandImport class.
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

//19 Apr 2006 - pjm
//Changed code to reduce compiler warnings.
//Added TestPoints function for debug
//Added VARIANT_BOOL usage to AddData() function
//SCR00217 - Add fix for returning all 18 "Channels" of Grand data in
//	GetDataByStation(), the floats version. SK did this  part.
//DR00280 fix - SCR00217 was not quite completed.  Added additional "Gets" to
//	GetDataByStation(), the doubles version.

///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to wrap all calls to the GRANDCom object.
//
//	Functions:
//		LocalCoInitialize	
//		LocalCoUninitialize
//		OpenGrandImportDatabase
//		GrandImport::CGrandImport
//		CGrandImport::~CGrandImport
//		CGrandImport::OpenDatabase
//		CGrandImport::InitializeDatabase
//		CGrandImport::CloseDatabase
//		CGrandImport::AddData
//		CGrandImport::GetDbStartDay
//		CGrandImport::GetDbEndDay
//		CGrandImport::GetDataStatistics
//		CGrandImport::GetDataByChannel
//		CGrandImport::GetDataByStation
//		cGrandImport::GetDayStatistics		
//		CGrandImport::GetDataByDay
///////////////////////////////////////////////////////////////////////////
#include "RadBuild.h"
#include "ComDataStructures.h"
#include "GrandImport.h"
#include "imports.h"
#include "MyDateTime.h"
#include "SafeArrayUtil.h"
#include "TimeAlign.h"
#include "TypeDefinitions.h"
#include "ErrorProcessing.h"

extern int gnLoggingLevel;
extern bool gbEnableLogging;
extern bool gbQuietMode;

IGrandImportPtr pIGrandImport;

/*  DR 221 Releasing COM foundation incorrectly
///////////////////////////////////////////////////////////////////////////
//	Name:	LocalCoInitialize, LocalCoUninitialize
//
//	Description:
//	Calls the CoInitialize or CoUninitialize for Com stuff -- this allows 
//	the call from the rest of Rad and only does it once during each run of the program
//	
//	Declaration:
//	void LocalCoInitialize()
//	void LocalCoUninitialize()
//
//	Input:	none
//
//	Output:	bCoInitialize	boolean indicating state of CoInitialize stuff
//
//	Return:	none
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//////////////////////////////////////////////////////////////////
static int bCoInitialize = 0;
void LocalCoInitialize()
{
	//HRESULT hr;		// sk 04/21/2003
	if (!bCoInitialize) {
		CoInitialize(NULL);
	//	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	//	if (hr == S_OK) {
			bCoInitialize = 1;
	//	}
	}
//	AfxOleInit();
}

void LocalCoUninitialize()
{
	if (bCoInitialize == 1) {
		CoUninitialize();
		bCoInitialize = 0;
	}

}
*/

///////////////////////////////////////////////////////////////////////////
//	Name:	OpenGrandImportDatabase
//
//	Description:
//	Creates the GrandImportCom object and opens the database in the com object.
//	
//	Declaration:
//	int OpenGrandImportDatabase(const char* strDataStorePath, const char* strDefaultPath)
//
//	Input:	strDataStorePath	path where database to be used by GrandCom is located
//			strDefaultPath		path where any default parameters to be used by GrandCom are located
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
int OpenGrandImportDatabase(const short sFacilityID)
{
	if (!pglGrandImport->OpenDatabase(sFacilityID))
		return(0);
	else
		return (1);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGrandImport::CGrandImport(const char* szDefaultPath, const short sFac)
{
	m_sFacNum = sFac;

	mstrDataStorePath[0] = '\0';
	mstrDefaultPath[0] = '\0';

	strncpy(mstrDefaultPath, szDefaultPath, 256);
	pIGrandImport.CreateInstance(CLSID_GrandImport);

	if (!gbQuietMode && (pIGrandImport == NULL))
	{
		AfxMessageBox(
			"Could not Create Instance of GRAND Import component.\npIGrandImport = NULL.");
	}

	VARIANT var;
	VariantInit(&var);
	var.vt = VT_I4;
	
	long flags = 0;

	if (!gbEnableLogging)
		flags |= DISABLE_LOGGER;

	if (gbQuietMode)
		flags |= QUIET_MODE;

	flags |= gnLoggingLevel;

	var.lVal = flags;
	pIGrandImport->SetParameters(m_sFacNum, var);
	::VariantClear(&var);
}

CGrandImport::~CGrandImport()
{
	pIGrandImport = NULL;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	OpenDatabase
//
//	Description:
//	Set parameters necessary in GrandCom and opens the database to be used by GrandCom.
//	
//	Declaration:
//	int CGrandImport::OpenDatabase(const char* strDataStorePath)
//
//	Input:	strDataStorePath	path where database to be used by GrandCom is located
//			strDefaultPath		path where any default parameters to be used by GrandCom are located
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
bool CGrandImport::OpenDatabase(const short sFacilityID)
{
	if (pIGrandImport == NULL)
		return false;

	if (pIGrandImport->OpenDataStore(sFacilityID))
		return true;
	else
		return false;
}
/*

	int iStatus = 0;

	//if (strDataStorePath != NULL) {
	//	strcpy(mstrDataStorePath,strDataStorePath);
	//	pIGrandImport->SetDataStore(mstrDataStorePath);
	//}
	//if (mstrDefaultPath != NULL) {
	//	pIGrandImport->SetDefaultPath(mstrDefaultPath);
	//}
	
	if (pIGrandImport) 
		iStatus = pIGrandImport->OpenDataStore(sFacilityID);

	return(iStatus);
}
*/

///////////////////////////////////////////////////////////////////////////
//	Name:	InitializeDatabase
//
//	Description:
//	Wraps GrandCom InitializeDatabase method.
//	
//	Declaration:
//	int CGrandImport::InitializeDatabase(const char* strDatabasePath, const bool bQuestion)
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
int CGrandImport::InitializeDatabase(const short sFacilityID, const bool bQuestion)
{
	if (pIGrandImport == NULL)
		return (0);

	VARIANT_BOOL vbQuestion = bQuestion?VARIANT_TRUE:VARIANT_FALSE;

	if (pIGrandImport->InitializeDataStore(sFacilityID, vbQuestion))
		return (1);
	else
		return (0);
}
/*
	int iStatus;
	
	if (pIGrandImport) iStatus = pIGrandImport->InitializeDataStore(sFacilityID, (VARIANT_BOOL)bQuestion);

	return(iStatus);
}
*/

///////////////////////////////////////////////////////////////////////////
//	Name:	CloseDatabase
//
//	Description:
//	Wraps GrandCom CloseDataStore method.
//	
//	Declaration:
//	void CGrandImport::CloseDatabase()
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
void CGrandImport::CloseDatabase()
{
	if (pIGrandImport != NULL)
		pIGrandImport->CloseDataStore();
}


///////////////////////////////////////////////////////////////////////////
//	Name:	AddData
//
//	Description:
//	Wraps GrandCom AddData method.
//	
//	Declaration:
//	int CGrandImport::AddData(const short sFacID, const char* strFilename, BOOL bOverwrite, char** strResult)
//
//	Input:	strFilename		name of file to be added to GrandCom database
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
int CGrandImport::AddData(
	const short sFacID, 
	const char* strFilename, 
	BOOL bOverwrite, 
	char** strResult)
{
	*strResult = NULL;

	if (pIGrandImport == NULL)
		return (0);

	VARIANT_BOOL vbOverwrite = (bOverwrite?VARIANT_TRUE:VARIANT_FALSE);
	BSTR bstr = NULL;

	VARIANT_BOOL vbSuccess = pIGrandImport->AddData(sFacID, strFilename, vbOverwrite, &bstr);

	CString cs(bstr);
	int len = cs.GetLength();
	*strResult = new char[len+1];		// allocate storage
	strncpy(*strResult, LPCTSTR(cs),len);	// copy to storage -- caller must deallocate
	(*strResult)[len] = 0;
	SysFreeString(bstr);

	return (vbSuccess != VARIANT_FALSE);
}
/*
	int iStatus;
	BSTR temp1;
	USES_CONVERSION;
	
	if (pIGrandImport)
	{
		VARIANT_BOOL vbOverwrite = (bOverwrite?VARIANT_TRUE:VARIANT_FALSE);
		iStatus = pIGrandImport->AddData(sFacID, strFilename, vbOverwrite, &temp1);
	}

	int len = strlen(W2T(temp1));	// determine length of string 
	*strResult = new char[len+1];	// allocate storage
	strcpy(*strResult, W2T(temp1));	// copy to storage -- caller must deallocate

	return(iStatus);
}
*/


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDbStartDay
//
//	Description:
//	Wraps GrandCom GetDataStoreTimeRange method and extracts the start day in the db.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CGrandImport::GetDbStartDay(const short sFacID, const short sStaID, DATE *pdStartDay, const bool bErrDisplay)
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
//	13-Aug-2002	SFK		Returned DATE to rest of Rad Review
//////////////////////////////////////////////////////////////////
int CGrandImport::GetDbStartDay(const short sFacID, const short sStaID, DATE *pdStartDay, const bool bErrDisplay)
{
	*pdStartDay = 0.0;

	if (pIGrandImport == NULL)
		return (0);

	VARIANT_BOOL vbErrDisplay = bErrDisplay?VARIANT_TRUE:VARIANT_FALSE;
	DATE dDumDate;

	if (pIGrandImport->GetDataStoreTimeRange(sFacID, sStaID, vbErrDisplay, pdStartDay, &dDumDate))
		return (1);
	else
		return (0);
}

/*
	DATE dDumDate;

	*pdStartDay = 0.0;

	VARIANT_BOOL bStatus = false;
	if (pIGrandImport) bStatus = pIGrandImport->GetDataStoreTimeRange(sFacID, sStaID, bErrDisplay, pdStartDay, &dDumDate);
	
	return(bStatus);
}
*/


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDbEndDay
//
//	Description:
//	Wraps GrandCom GetDbEndDay method and extracts the end day in the db.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CGrandImport::GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay)
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
//	13-Aug-2002	SFK		Returned DATE to rest of Rad Review
//////////////////////////////////////////////////////////////////
int CGrandImport::GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay)
{
	*pdEndDay = 0.0;

	if (pIGrandImport == NULL)
		return (0);

	VARIANT_BOOL vbErrDisplay = bErrDisplay?VARIANT_TRUE:VARIANT_FALSE;
	DATE dDumDate;

	if (pIGrandImport->GetDataStoreTimeRange(sFacID, sStaID, vbErrDisplay, &dDumDate, pdEndDay))
		return(1);
	else
		return (0);
}

/*

	DATE dDumDate;

	*pdEndDay = 0.0;

	VARIANT_BOOL bStatus = false;
	if (pIGrandImport) bStatus = pIGrandImport->GetDataStoreTimeRange(sFacID,sStaID, bErrDisplay, &dDumDate, pdEndDay);

	return(bStatus);
}
*/

///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataStatistics
//
//	Description:
//	Wraps GrandCom GetDataStatistics method.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CGrandImport::GetDataStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			ulStart		DATE for start of search
//			ulEnd		DATE for end of search
//
//	Output:	pdStart	DATE of first data point in the interval
//			pdEnd		DATE of last data point in the interval
//			pulNumPts	number of data points between and including pdStart to pdEnd
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	30-Apr-2002 SFK		Received DATE data from COMs
//	13-Aug-2002	SFK		Returned DATE to rest of Rad Review
//////////////////////////////////////////////////////////////////
int CGrandImport::GetDataStatistics(
	const short sFacID, 
	const short sStaID, 
	const short sChanID, 
	const DATE dStart, 
	const DATE dEnd, 
	DATE *pdActualStart, 
	DATE *pdActualEnd, 
	unsigned long *pulNumPts)
{
	*pdActualStart = 0.0;
	*pdActualEnd = 0.0;
	*pulNumPts = 0;

	if (pIGrandImport == NULL)
		return (0);

	long lNumPts;

	if (pIGrandImport->GetDataStatistics(
			sFacID, sStaID, sChanID, dStart, dEnd, pdActualStart, pdActualEnd, &lNumPts))
	{
		*pulNumPts = lNumPts;
		return (1);
	}
	else
	{
		return(0);
	}
}

/*
	VARIANT_BOOL bStatus = false;
	if (pIGrandImport) bStatus = pIGrandImport->GetDataStatistics(sFacID, sStaID, sChanID, dStart, dEnd, pdActualStart, pdActualEnd, &lNumPts);
	
	//	convert timestamps back to unsigned longs for now
	if (bStatus) { //extract info
		*pulNumPts = lNumPts;
	}

	return(bStatus);
}
*/


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByChannel
//
//	Description:
//	Wraps GrandCom GetDataByChannel method.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CGrandImport::GetDataByChannel(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, double *pdX, double *pdY, unsigned long *pulNumPts, DATE *pdNextTime)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			sChanID		Channel number at station (must be 0-6)
//			ulStart		DATE for start of search
//			ulEnd		DATE for end of search
//
//	Output:	pdX			array containing DATEs of all data in interval including endpoints
//			pdY			array containing data values of all data in interval including endpoints
//			pulNumPts	number of entries in the pdX or pdY array
//			pulNextTime	DATE of the next possible data point following the endpoint
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	30-Apr-2002 SFK		Received DATE data from COMs
//	13-Aug-2002	SFK		Returned DATE to rest of Rad Review
//	28-Oct-2004	SFK		Changed to receive all data in the SAFEARRAY and parse out timestamps, data and next time
//////////////////////////////////////////////////////////////////
int CGrandImport::GetDataByChannel(
	const short sFacID, 
	const short sStaID, 
	const short sChanID, 
	const DATE dStart, 
	const DATE dEnd, 
	double *pdX, 
	double *pdY, 
	unsigned long *pulNumPts, 
	DATE *pdNextTime)
{
	*pdNextTime = 0.0;

	if (pIGrandImport == NULL)
		return (0);

	if (*pulNumPts == 0) 
		return (1);

	long lIndex;

	VARIANT vData;
	VariantInit(&vData);
	vData.vt = VT_I4;
	vData.lVal = *pulNumPts;

	//<<QA>> put time align switch on this eventually
	VARIANT_BOOL vb = pIGrandImport->GetDataByChannel(sFacID, sStaID, sChanID, dStart, dEnd, &vData);
	
	// Extract data from the SAFEARRAY and return to Rad as in previous calls
	if (vb && (vData.vt == (VT_VARIANT | VT_ARRAY))) 
	{
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vData.parray, 1, &lUBound);
		//pjm 25-Jan-06 Added explicit cast to get rid of compiler warning
		if (lUBound > (long)*pulNumPts)		// got back more points than expected only return first *pulNumPts
			lUBound = (long)*pulNumPts - 1;

		for (lIndex=0; lIndex<lUBound; lIndex++) 
		{
			SA_GetElem(vData.parray, lIndex, 0, &(pdX[lIndex]));		// timestamp in column 1
			SA_GetElem(vData.parray, lIndex, 1, &(pdY[lIndex]));		// data in column 2
		}

		*pulNumPts = lIndex;									// number of points returned
		SA_GetElem(vData.parray, lIndex, 0, pdNextTime);		// timestamp of next data point
	}
	else
	{
		vb = VARIANT_FALSE;
	}

	VariantClear(&vData);

	if (vb)
		return (1);
	else
		return (0);
}
/*

	VARIANT vData;
	long lIndex;

	*pdNextTime = 0.0;
	if (*pulNumPts == 0) return(1);

	vData.lVal = *pulNumPts;
	VARIANT_BOOL bStatus = false;
	if (pIGrandImport) bStatus = pIGrandImport->GetDataByChannel(sFacID, sStaID, sChanID, dStart, dEnd, &vData);
	
	// Extract data from the SAFEARRAY and return to Rad as in previous calls
	if ((bStatus && (vData.vt == (VT_VARIANT | VT_ARRAY)))) {
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vData.parray, 1, &lUBound);
		//pjm 25-Jan-06 Added explicit cast to get rid of compiler warning
		if (lUBound > (long)*pulNumPts) {		// got back more points than expected only return first *pulNumPts
			lUBound = (long)*pulNumPts - 1;
		}
		for (lIndex=0; lIndex<lUBound; lIndex++) {
			SA_GetElem(vData.parray, lIndex, 0, &(pdX[lIndex]));		// timestamp in column 1
			SA_GetElem(vData.parray, lIndex, 1, &(pdY[lIndex]));		// data in column 2
		}

		*pulNumPts = lIndex;									// number of points returned
		SA_GetElem(vData.parray, lIndex, 0, pdNextTime);		// timestamp of next data point
		SafeArrayDestroy(vData.parray);
	}
	return(bStatus);
}
*/


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByStation
//
//	Description:
//	Wraps GrandCom GetDataByStation method
//	The first instance of the function returns data in the instrument's native format.
//	The second instance of the function returns data in a double array -- to support most
//	of the analysis routines.
//	
//	Declaration:
//	int CGrandImport::GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_FLOAT_DATA_PT1 *pfData, unsigned long *pulNumPts, DATE *pdNextTime)
//	int CGrandImport::GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_DOUBLE_DATA_PT1 *pdData, unsigned long *pulNumPts, DATE *pdNextTime)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			ulStart		DATE of interval start to retrieve Grand data records
//			ulEnd		DATE of interval end to retrieve Grand data records
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
//  05-Jun-2002 SFK		Changed to receive DATE from Rad
//	13-Aug-2002	SFK		Returned DATE to rest of Rad Review
//	24-Mar-2003	SFK		Added function that returns double version of data
//	15-Oct-2003	SFK		Modified to return status bits as data values
//	28-Oct-2004	SFK		Changed to receive all data in the SAFEARRAY
//	22-Feb-2006 SFK		Add returning all 18 "channels" of Grand data. (SCR00217)
//////////////////////////////////////////////////////////////////
// Get station data as floats
int CGrandImport::GetDataByStation(
	const short sFacID, 
	const short sStaID, 
	const DATE dStart, 
	const DATE dEnd, 
	struct WHOLE_FLOAT_DATA_PT1 *pfData, 
	unsigned long *pulNumPts, 
	DATE *pdNextTime)
{
	*pdNextTime = 0.0;

	if (pIGrandImport == NULL)
		return (0);

	if (*pulNumPts == 0) 
		return(1);
	
	VARIANT vData;
	VariantInit(&vData);
	vData.vt = VT_I4;
	vData.lVal = *pulNumPts;

	//int iStatus = 0;
	long lIndex;

	VARIANT_BOOL vb;

	//<<QA>>put the Time Align call back in, too (with the boolean)
	if (false) //put time align bool here instead of the "false"
	{
		if (!pglTimeAlign->GetDataByStation(sFacID, sStaID, dStart, dEnd, &vData, GRAND_TYPE, pulNumPts))
			vb = VARIANT_FALSE;
		else
			vb = VARIANT_TRUE;
	}
	else
	{
		vb = pIGrandImport->GetDataByStation (sFacID, (long)sStaID, dStart, dEnd, &vData);
	}

	if (vb) 
	{	// extract the data
		// now pull the data back out
		if (vData.vt == (VT_VARIANT | VT_ARRAY)) 
		{
			long lUBound;
			// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
			// so the number of members in the SAFEARRAY is lUBound + 1;
			SafeArrayGetUBound(vData.parray, 1, &lUBound);
			//pjm 25-Jan-06 Added explicit cast to get rid of compiler warning
			if (lUBound > (long)*pulNumPts)		// got back more points than expected only return first *pulNumPts
				lUBound = (long)*pulNumPts - 1;

			for (lIndex=0; lIndex<lUBound; lIndex++) {
				SA_GetElem(vData.parray, lIndex, 0, &(pfData[lIndex].dJulianTime));
				SA_GetElem(vData.parray, lIndex, 1, (long *)&(pfData[lIndex].ulStatus));
				SA_GetElem(vData.parray, lIndex, 2, &(pfData[lIndex].fData[0]));		// neutron A
				SA_GetElem(vData.parray, lIndex, 3, &(pfData[lIndex].fData[1]));		// neutron B
				SA_GetElem(vData.parray, lIndex, 4, &(pfData[lIndex].fData[2]));		// neutron C
				SA_GetElem(vData.parray, lIndex, 5, &(pfData[lIndex].fData[3]));		// gamma 1
				SA_GetElem(vData.parray, lIndex, 6, &(pfData[lIndex].fData[4]));		// gamma 2
				SA_GetElem(vData.parray, lIndex, 7, &(pfData[lIndex].fData[5]));		// gamma 1 unc
				SA_GetElem(vData.parray, lIndex, 8, &(pfData[lIndex].fData[6]));		// gamma 2 unc
				SA_GetElem(vData.parray, lIndex, 9, &(pfData[lIndex].fData[7]));		// ulStatus bit 0 (In MII)
				SA_GetElem(vData.parray, lIndex, 10, &(pfData[lIndex].fData[8]));		// ulStatus bit 1 (No authentication)
				SA_GetElem(vData.parray, lIndex, 11, &(pfData[lIndex].fData[9]));		// ulStatus bit 2 (Cold start)
				SA_GetElem(vData.parray, lIndex, 12, &(pfData[lIndex].fData[10]));		// ulStatus bit 3 (Taking offsets)
				SA_GetElem(vData.parray, lIndex, 13, &(pfData[lIndex].fData[11]));		// ulStatus bit 4 (Over threshold)
				SA_GetElem(vData.parray, lIndex, 14, &(pfData[lIndex].fData[12]));		// ulStatus bit 5 (Changing signal)
				SA_GetElem(vData.parray, lIndex, 15, &(pfData[lIndex].fData[13]));		// ulStatus bit 6 (Battery low)
				SA_GetElem(vData.parray, lIndex, 16, &(pfData[lIndex].fData[14]));		// ulStatus bit 7 (AC power loss)
				//SCR 217 - Support extra MG channels for unusual gamma conditions
				//Also DR 249
				SA_GetElem(vData.parray, lIndex, 17, &(pfData[lIndex].fData[15]));		// ulStatus bit 16 (Taking offsets)
				SA_GetElem(vData.parray, lIndex, 18, &(pfData[lIndex].fData[16]));		// ulStatus bit 17 (Invalid gammas)
				SA_GetElem(vData.parray, lIndex, 19, &(pfData[lIndex].fData[17]));		// ulStatus bit 18 (Saturated gammas)
				SA_GetElem(vData.parray, lIndex, 20, &(pfData[lIndex].dElapsedTime));
			}
			SA_GetElem(vData.parray, lIndex, 0, pdNextTime); // timestamp of next data point
			//SafeArrayDestroy(vData.parray);
		}
	}  //if vb

	VariantClear(&vData);

	if (vb) 
		return(1);
	else
		return(0);
}
/*
	
	VARIANT vData;
	VariantInit(&vData);
	int iStatus = 0;
	long lIndex;

	// set default returns and check if any points requested.
	*pdNextTime = 0.0;
	if (*pulNumPts == 0) return(1);

	vData.lVal = *pulNumPts;
	iStatus = pglTimeAlign->GetDataByStation(sFacID, sStaID, dStart, dEnd, &vData, GRAND_TYPE, pulNumPts);

	if (iStatus) {	// extract the data
		// now pull the data back out
		if (vData.vt == (VT_VARIANT | VT_ARRAY)) {
			long lUBound;
			// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
			// so the number of members in the SAFEARRAY is lUBound + 1;
			SafeArrayGetUBound(vData.parray, 1, &lUBound);
			//pjm 25-Jan-06 Added explicit cast to get rid of compiler warning
			if (lUBound > (long)*pulNumPts) {		// got back more points than expected only return first *pulNumPts
				lUBound = (long)*pulNumPts - 1;
			}
			for (lIndex=0; lIndex<lUBound; lIndex++) {
				SA_GetElem(vData.parray, lIndex, 0, &(pfData[lIndex].dJulianTime));
				SA_GetElem(vData.parray, lIndex, 1, (long *)&(pfData[lIndex].ulStatus));
				SA_GetElem(vData.parray, lIndex, 2, &(pfData[lIndex].fData[0]));		// neutron A
				SA_GetElem(vData.parray, lIndex, 3, &(pfData[lIndex].fData[1]));		// neutron B
				SA_GetElem(vData.parray, lIndex, 4, &(pfData[lIndex].fData[2]));		// neutron C
				SA_GetElem(vData.parray, lIndex, 5, &(pfData[lIndex].fData[3]));		// gamma 1
				SA_GetElem(vData.parray, lIndex, 6, &(pfData[lIndex].fData[4]));		// gamma 2
				SA_GetElem(vData.parray, lIndex, 7, &(pfData[lIndex].fData[5]));		// gamma 1 unc
				SA_GetElem(vData.parray, lIndex, 8, &(pfData[lIndex].fData[6]));		// gamma 2 unc
				SA_GetElem(vData.parray, lIndex, 9, &(pfData[lIndex].fData[7]));		// ulStatus bit 0 (In MII)
				SA_GetElem(vData.parray, lIndex, 10, &(pfData[lIndex].fData[8]));		// ulStatus bit 1 (No authentication)
				SA_GetElem(vData.parray, lIndex, 11, &(pfData[lIndex].fData[9]));		// ulStatus bit 2 (Cold start)
				SA_GetElem(vData.parray, lIndex, 12, &(pfData[lIndex].fData[10]));		// ulStatus bit 3 (Taking offsets)
				SA_GetElem(vData.parray, lIndex, 13, &(pfData[lIndex].fData[11]));		// ulStatus bit 4 (Over threshold)
				SA_GetElem(vData.parray, lIndex, 14, &(pfData[lIndex].fData[12]));		// ulStatus bit 5 (Changing signal)
				SA_GetElem(vData.parray, lIndex, 15, &(pfData[lIndex].fData[13]));		// ulStatus bit 6 (Battery low)
				SA_GetElem(vData.parray, lIndex, 16, &(pfData[lIndex].fData[14]));		// ulStatus bit 7 (AC power loss)
				//SCR00217 - Support extra MG channels for unusual gamma conditions
				SA_GetElem(vData.parray, lIndex, 17, &(pfData[lIndex].fData[15]));		// ulStatus bit 16 (Taking offsets)
				SA_GetElem(vData.parray, lIndex, 18, &(pfData[lIndex].fData[16]));		// ulStatus bit 17 (Invalid gammas)
				SA_GetElem(vData.parray, lIndex, 19, &(pfData[lIndex].fData[17]));		// ulStatus bit 18 (Saturated gammas)
				SA_GetElem(vData.parray, lIndex, 20, &(pfData[lIndex].dElapsedTime));
			}
			SA_GetElem(vData.parray, lIndex, 0, pdNextTime); // timestamp of next data point
			SafeArrayDestroy(vData.parray);
		}
	}
	return(iStatus);
}
*/


//  Get station data as doubles
int CGrandImport::GetDataByStation(
	const short sFacID, 
	const short sStaID, 
	const DATE dStart, const DATE dEnd, 
	struct WHOLE_DOUBLE_DATA_PT1 *pdData, 
	unsigned long *pulNumPts, 
	DATE *pdNextTime)
{

	*pdNextTime = 0.0;

	if (pIGrandImport == NULL)  //need to include check the time align pointer in this.
		return (0);

	if (*pulNumPts == 0) 
		return(1);
	
	VARIANT vData;
	VariantInit(&vData);

	vData.vt = VT_I4;
	vData.lVal = *pulNumPts;

	//int iStatus = 0;
	long lIndex;

	VARIANT_BOOL vb;

	//<<QA>>put the Time Align call back in, too (with the boolean)
	if (false) //put time align bool here instead of the "false"
	{
		if (!pglTimeAlign->GetDataByStation(sFacID, sStaID, dStart, dEnd, &vData, GRAND_TYPE, pulNumPts))
			vb = VARIANT_FALSE;
		else
			vb = VARIANT_TRUE;
	}
	else
	{
		vb = pIGrandImport->GetDataByStation (sFacID, (long)sStaID, dStart, dEnd, &vData);
	}

	// extract the data
	if (vb) // && (vData.vt == (VT_VARIANT | VT_ARRAY)))
	{
		if (vData.vt == (VT_VARIANT | VT_ARRAY))
		{
			// now pull the data back out
			long lUBound;
			// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
			// so the number of members in the SAFEARRAY is lUBound + 1;
			SafeArrayGetUBound(vData.parray, 1, &lUBound);
			//pjm 25-Jan-06 Added explicit cast to get rid of compiler warning
			if (lUBound > (long)*pulNumPts) {		// got back more points than expected only return first *pulNumPts
				lUBound = (long)*pulNumPts - 1;
			}
			for (lIndex=0; lIndex<lUBound; lIndex++) {
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
				SA_GetElem(vData.parray, lIndex, 10, &(pdData[lIndex].dData[8]));
				SA_GetElem(vData.parray, lIndex, 11, &(pdData[lIndex].dData[9]));
				SA_GetElem(vData.parray, lIndex, 12, &(pdData[lIndex].dData[10]));
				SA_GetElem(vData.parray, lIndex, 13, &(pdData[lIndex].dData[11]));
				SA_GetElem(vData.parray, lIndex, 14, &(pdData[lIndex].dData[12]));
				SA_GetElem(vData.parray, lIndex, 15, &(pdData[lIndex].dData[13]));
				SA_GetElem(vData.parray, lIndex, 16, &(pdData[lIndex].dData[14]));
				//DR 280 against 3.2.0.4 - these 3 lines were left out of 3.2.0.4 for SCR 217
				//SCR 217 - Support extra MG channels for unusual gamma conditions.
				SA_GetElem(vData.parray, lIndex, 17, &(pdData[lIndex].dData[15]));
				SA_GetElem(vData.parray, lIndex, 18, &(pdData[lIndex].dData[16]));
				SA_GetElem(vData.parray, lIndex, 19, &(pdData[lIndex].dData[17]));
				SA_GetElem(vData.parray, lIndex, 20, &(pdData[lIndex].dElapsedTime));
			}
			SA_GetElem(vData.parray, lIndex, 0, pdNextTime); // timestamp of next data point
			//SafeArrayDestroy(vData.parray);
		}
	}  //if vb

	VariantClear(&vData);

	if (vb) 
		return(1);
	else
		return(0);
}
/*
	VARIANT vData;
	VariantInit(&vData);
	int iStatus = 0;
	long lIndex;

	// set default returns and check if any points requested.
	*pdNextTime = 0.0;
	if (*pulNumPts == 0) return(1);

	vData.lVal = *pulNumPts;
	iStatus = pglTimeAlign->GetDataByStation(sFacID, sStaID, dStart, dEnd, &vData, GRAND_TYPE, pulNumPts);

	// extract the data
	if (iStatus && (vData.vt == (VT_VARIANT | VT_ARRAY))){
	// now pull the data back out
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vData.parray, 1, &lUBound);
		//pjm 25-Jan-06 Added explicit cast to get rid of compiler warning
		if (lUBound > (long)*pulNumPts) {		// got back more points than expected only return first *pulNumPts
			lUBound = (long)*pulNumPts - 1;
		}
		for (lIndex=0; lIndex<lUBound; lIndex++) {
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
			SA_GetElem(vData.parray, lIndex, 10, &(pdData[lIndex].dData[8]));
			SA_GetElem(vData.parray, lIndex, 11, &(pdData[lIndex].dData[9]));
			SA_GetElem(vData.parray, lIndex, 12, &(pdData[lIndex].dData[10]));
			SA_GetElem(vData.parray, lIndex, 13, &(pdData[lIndex].dData[11]));
			SA_GetElem(vData.parray, lIndex, 14, &(pdData[lIndex].dData[12]));
			SA_GetElem(vData.parray, lIndex, 15, &(pdData[lIndex].dData[13]));
			SA_GetElem(vData.parray, lIndex, 16, &(pdData[lIndex].dData[14]));
			//DR00280 against 3.2.0.4 - these 3 lines were left out of 3.2.0.4 for SCR 217
			//SCR00217 - Support extra MG channels for unusual gamma conditions.
			SA_GetElem(vData.parray, lIndex, 17, &(pdData[lIndex].dData[15]));
			SA_GetElem(vData.parray, lIndex, 18, &(pdData[lIndex].dData[16]));
			SA_GetElem(vData.parray, lIndex, 19, &(pdData[lIndex].dData[17]));
			SA_GetElem(vData.parray, lIndex, 20, &(pdData[lIndex].dElapsedTime));
		}
		SA_GetElem(vData.parray, lIndex, 0, pdNextTime); // timestamp of next data point
		SafeArrayDestroy(vData.parray);
	}
	return(iStatus);
}
*/


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDayStatistics
//
//	Description:
//	Wraps GrandCom GetDayStatistics GrandCom method.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CGrandImport::GetDayStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			ulStart		DATE of interval start to retrieve Grand data records
//			ulEnd		DATE of interval end to retrieve Grand data records
//
//	Output:	pulNumPts	actual number of data points found in the interval
//			pdStart	DATE of first day in interval
//			pdEnd		DATE of last day in interval
//
//	Return:	1(all done)/0(error)
//	
//  date    /	author	revision
//  -----------------	--------
//	19-Feb-2002	SFK		Created
//	19-Mar-2002 SFK		Removed DATE references -- for short term use NIS5 timebase
//	30-Apr-2002 SFK		Received DATE data from COMs
//	13-Aug-2002	SFK		Returned DATE to rest of Rad Review
//////////////////////////////////////////////////////////////////
int CGrandImport::GetDayStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts)
{
	*pdActualStart = 0.0;
	*pdActualEnd = 0.0;
	*pulNumPts = 0;

	if (pIGrandImport == NULL)
		return (0);

	long lNumPts;

	if(pIGrandImport->GetDayStatistics(sFacID, sStaID, dStart, dEnd, pdActualStart, pdActualEnd, &lNumPts))
	{
		// convert timestamps back to unsigned longs for now
		*pulNumPts = (unsigned long)lNumPts;
		return (1);
	}
	else
	{
		return (0);
	}
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByDay
//
//	Description:
//	Wraps GrandCom GetDataByDay method.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CGrandImport::GetDataByDay(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			ulStart		DATE of interval start to retrieve Grand data records
//			ulEnd		DATE of interval end to retrieve Grand data records
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
int CGrandImport::GetDataByDay(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays)
{
	VARIANT vDayData;
	VariantInit(&vDayData);

	long lIndex;
	VARIANT_BOOL bStatus = false;

	//<<QA>> this ALWAYS returns.  ???
	*pulNumDays = 0;
	if (*pulNumDays == 0) return(1);

	if (pIGrandImport) bStatus = pIGrandImport->GetDaySummaryData(sFacID, (short)sStaID, dStart, dEnd, &vDayData);
	
	// now pull the data back out
	if ((bStatus)  && (vDayData.vt == (VT_VARIANT | VT_ARRAY))) {	// is the type of SafeArray we are expecting 
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vDayData.parray, 1, &lUBound);
		//pjm 25-Jan-06 Added explicit cast to get rid of compiler warning
		if (lUBound > (long)*pulNumDays) {		// got back more points than expected only return first *pulNumPts
			lUBound = (long)*pulNumDays - 1;
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

// 28-Oct-2004	SFK	Added sFacilityID arg to calling parms of function and COM call
int CGrandImport::BackupDatabase(const short sFacilityID, const char *strPath, const bool bQuestion, const bool bPrompt)
{
	if (pIGrandImport == NULL)
		return (0);

	VARIANT_BOOL vbQuestion = bQuestion?VARIANT_TRUE:VARIANT_FALSE;
	VARIANT_BOOL vbPrompt = bPrompt?VARIANT_TRUE:VARIANT_FALSE;
	
	if (pIGrandImport->BackupDataStore(sFacilityID, strPath, vbQuestion, vbPrompt))
		return 1;
	else
		return 0;
}

// 28-Oct-2004	SFK	Added sFacilityID arg to calling parms of function and COM call
int CGrandImport::RestoreDatabase(const short sFacilityID, const char *strPath, const char *strDestPath, const bool bQuestion, const bool bPrompt)
{
	if (pIGrandImport == NULL)
		return (0);

	VARIANT_BOOL vbQuestion = bQuestion?VARIANT_TRUE:VARIANT_FALSE;
	VARIANT_BOOL vbPrompt = bPrompt?VARIANT_TRUE:VARIANT_FALSE;
	
	if (pIGrandImport->RestoreDataStore(sFacilityID, strPath, vbQuestion, vbPrompt))
		return 1;
	else
		return 0;
}
	/*
	int iStatus;
	
	if (pIGrandImport) iStatus = pIGrandImport->RestoreDataStore(sFacilityID, strPath, (VARIANT_BOOL)bQuestion, (VARIANT_BOOL)bPrompt);
	return(iStatus);
}
*/

// 28-Oct-2004	SFK	Added sFacilityID arg to calling parms of function and COM call
void CGrandImport::GetDataStoreSize(const short sFacilityID, double *pdSizeInBytes)
{
	*pdSizeInBytes = 0;

	if (pIGrandImport) pIGrandImport->GetDataStoreSize(sFacilityID, pdSizeInBytes);
}
