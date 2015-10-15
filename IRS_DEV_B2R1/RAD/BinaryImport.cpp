// BinaryImport.cpp: implementation of the CBinaryImport class.
//
//////////////////////////////////////////////////////////////////////

//19 Apr 2006 - pjm - changed code to reduce some compiler warnings.

///////////////////////////////////////////////////////////////////////////
//	Copyright, 2003, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
//	This file contains the routines to wrap all calls to the BinaryCom object.
//
//	Functions:
//		LocalCoInitialize	
//		LocalCoUninitialize
//		OpenBinaryImportDatabase
//		BinaryImport::CBinaryImport
//		CBinaryImport::~CBinaryImport
//		CBinaryImport::OpenDatabase
//		CBinaryImport::InitializeDatabase
//		CBinaryImport::CloseDatabase
//		CBinaryImport::AddData
//		CBinaryImport::GetDbStartDay
//		CBinaryImport::GetDbEndDay
//		CBinaryImport::GetDataStatistics
//		CBinaryImport::GetDataByChannel
//		CBinaryImport::GetDataByStation
//		CBinaryImport::GetDayStatistics		
//		CBinaryImport::GetDataByDay
///////////////////////////////////////////////////////////////////////////

#include "RadBuild.h"
#include "afxconv.h"
#include "ComDataStructures.h"

#include "imports.h"
#include "BinaryImport.h"		// moved afer sk 04-16-2003
#include "MyDateTime.h"
#include "SafeArrayUtil.h"
//#include "TimeAlign.h"
#include "TypeDefinitions.h"
#include "ErrorProcessing.h"

IBinaryImportPtr pIBinaryImport;

extern bool gbQuietMode;
extern bool gbEnableLogging;
extern int  gnLoggingLevel;
extern short glsFacNum;

//04-Jan-2006 SFK Added check "if (pIBinaryImport)" before each call to COM

///////////////////////////////////////////////////////////////////////////
//	Name:	OpenBinaryImportDatabase
//
//	Description:
//	Creates the BinaryImportCom object and opens the database in the com object.
//	
//	Declaration:
//	int OpenBinaryImportDatabase(const char* strDataStorePath, const char* strDefaultPath)
//
//	Input:	strDataStorePath	path where database to be used by BinaryCom is located
//			strDefaultPath		path where any default parameters to be used by BinaryCom are located
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
int OpenBinaryImportDatabase(const short sFacilityID)
{
	if (pglBinaryImport->OpenDatabase(sFacilityID))
		return(1);
	else
		return (0);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBinaryImport::CBinaryImport(const char* szDefaultPath)
{

	mstrDataStorePath[0] = '\0';
	strncpy(mstrDefaultPath, szDefaultPath, 256);
	pIBinaryImport.CreateInstance(CLSID_BinaryImport);

	if (pIBinaryImport == NULL)
	{
		if (!gbQuietMode)
		{
			AfxMessageBox(
				"Could not Create Instance of Binary Import component.\npIGrandImport = NULL.");
		}

		return;
	}

	VARIANT var;
	::VariantInit(&var);
	var.vt = VT_I4;
	
	long flags = 0;

	if (!gbEnableLogging)
		flags |= DISABLE_LOGGER;

	if (!gbQuietMode)
		flags |= QUIET_MODE;

	flags |= gnLoggingLevel;

	var.lVal = flags;
	pIBinaryImport->SetParameters(glsFacNum, var);
	::VariantClear(&var);
}
CBinaryImport::~CBinaryImport()
{
	pIBinaryImport = NULL;
}
///////////////////////////////////////////////////////////////////////////
//	Name:	OpenDatabase
//
//	Description:
//	Set parameters necessary in BinaryCom and opens the database to be used by BinaryCom.
//	
//	Declaration:
//	int CBinaryImport::OpenDatabase(const char* strDataStorePath, const char* strDefaultPath)
//
//	Input:	strDataStorePath	path where database to be used by BinaryCom is located
//			strDefaultPath		path where any default parameters to be used by BinaryCom are located
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
bool CBinaryImport::OpenDatabase(const short sFacilityID)
{
	if (pIBinaryImport->OpenDataStore(sFacilityID))
		return true;
	else
		return false;
}
///////////////////////////////////////////////////////////////////////////
//	Name:	InitializeDatabase
//
//	Description:
//	Wraps BinaryCom InitializeDatabase method.
//	
//	Declaration:
//	int CBinaryImport::InitializeDatabase(const char* strDatabasePath)
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
int CBinaryImport::InitializeDatabase(const short sFacilityID, const bool bQuestion)
{
	VARIANT_BOOL vbQuestion = bQuestion ? VARIANT_TRUE:VARIANT_FALSE;

	if (pIBinaryImport->InitializeDataStore(sFacilityID, vbQuestion))
		return 1;
	else
		return 0;
}
///////////////////////////////////////////////////////////////////////////
//	Name:	CloseDatabase
//
//	Description:
//	Wraps BinaryCom CloseDataStore method.
//	
//	Declaration:
//	void CBinaryImport::CloseDatabase()
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
void CBinaryImport::CloseDatabase()
{
	if (pIBinaryImport) 
		pIBinaryImport->CloseDataStore();
}


///////////////////////////////////////////////////////////////////////////
//	Name:	AddData
//
//	Description:
//	Wraps BinaryCom AddData method.
//	
//	Declaration:
//	int CBinaryImport::AddData(const char* strFilename, char* strResult)
//
//	Input:	strFilename		name of file to be added to BinaryCom database
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
int CBinaryImport::AddData(const short sFacID, const char* strFilename, BOOL bOverwrite, char** strResult)
{
	if (pIBinaryImport == NULL)
		return 0;

	int iStatus;
	BSTR temp1;
	USES_CONVERSION;
	
	iStatus = pIBinaryImport->AddData(sFacID, strFilename, bOverwrite, &temp1);
	int len = strlen(W2T(temp1));	// determine length of string 
	*strResult = new char[len+1];	// allocate storage
	strcpy(*strResult, W2T(temp1));	// copy to storage -- caller must deallocate

	return(iStatus);
}
///////////////////////////////////////////////////////////////////////////
//	Name:	GetDbStartDay
//
//	Description:
//	Wraps BinaryCom GetDataStoreTimeRange method and extracts the start day in the db.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CBinaryImport::GetDbStartDay(const short sFacID, const short sStaID, DATE *pdStartDay, const bool bErrDisplay)
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
int CBinaryImport::GetDbStartDay(const short sFacID, const short sStaID, DATE *pdStartDay, const BOOL bErrDisplay)
{
	DATE dDumDate;
	*pdStartDay = 0.0;
	VARIANT_BOOL vbErrDisplay = bErrDisplay?VARIANT_TRUE:VARIANT_FALSE;

	if (pIBinaryImport->GetDataStoreTimeRange( sFacID, sStaID, 
		vbErrDisplay, pdStartDay, &dDumDate))
		return 1;
	else
		return 0;
}
///////////////////////////////////////////////////////////////////////////
//	Name:	GetDbEndDay
//
//	Description:
//	Wraps BinaryCom GetDbEndDay method and extracts the end day in the db.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CBinaryImport::GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const bool bErrDisplay)
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
int CBinaryImport::GetDbEndDay(const short sFacID, const short sStaID, DATE *pdEndDay, const BOOL bErrDisplay)
{
	DATE dDum;
	VARIANT_BOOL vb = bErrDisplay?VARIANT_TRUE:VARIANT_FALSE;

	if (pIBinaryImport->GetDataStoreTimeRange(sFacID,sStaID, vb, &dDum, pdEndDay))
	{
		return 1;
	}
	else
	{
		*pdEndDay = 0.0;
		return 0;
	}
}
///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataStatistics
//
//	Description:
//	Wraps BinaryCom GetDataStatistics method.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CBinaryImport::GetDataStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdStart, DATE *pdEnd, unsigned long *pulNumPts)
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
int CBinaryImport::GetDataStatistics(
	const short sFacID, 
	const short sStaID, 
	const short sChanID, 
	const DATE dStart, 
	const DATE dEnd, 
	DATE *pdActualStart, 
	DATE *pdActualEnd, 
	unsigned long *pulNumPts)
{
	long lNumPts;

	if (pIBinaryImport->GetDataStatistics(sFacID, sStaID, sChanID, dStart, dEnd, 
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
///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByChannel
//
//	Description:
//	Wraps BinaryCom GetDataByChannel method.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CBinaryImport::GetDataByChannel(const short sFacID, const short sStaID, const short sChanID, const DATE dStart, const DATE dEnd, double *pdX, double *pdY, unsigned long *pulNumPts, DATE *pdNextTime)
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
int CBinaryImport::GetDataByChannel(
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
	*pdNextTime = 0.0;
	if (*pulNumPts == 0) 
		return 1;

	if (pIBinaryImport == NULL)
		return 0;

	VARIANT vData;
	VariantInit(&vData);

	vData.vt = VT_I8;
	vData.lVal = *pulNumPts;

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID 
	VARIANT_BOOL vb = pIBinaryImport->GetDataByChannel(sFacID, sStaID, sChanID, dStart, dEnd, &vData);

	// Extract data from the SAFEARRAY and return to Rad as in previous calls
	if (vb && (vData.vt == (VT_VARIANT | VT_ARRAY))) 
	{
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vData.parray, 1, &lUBound);
		//pjm 25-Jan-2006 Added explicit cast to eliminate compiler warning.
		if (lUBound > (long)*pulNumPts)		// got back more points than expected only return first *pulNumPts
			lUBound = (long)*pulNumPts - 1;

		long lIndex;
		for (lIndex=0; lIndex<lUBound; lIndex++) 
		{
			SA_GetElem(vData.parray, lIndex, 0, &(pdX[lIndex]));		// timestamp in column 1
			SA_GetElem(vData.parray, lIndex, 1, &(pdY[lIndex]));		// data in column 2
		}
		*pulNumPts = lIndex;									// number of points returned
		SA_GetElem(vData.parray, lIndex, 0, pdNextTime);		// timestamp of next data point
	}

	VariantClear(&vData);

	if (vb)
		return 1;
	else
		return 0;
}
/*
int CBinaryImport::GetDataByChannel(
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
	VARIANT vData;
	::VariantInit(&vData); //pjm

	long lIndex;
	*pdNextTime = 0.0;

	if (*pulNumPts == 0) 
		return(1);

	vData.lVal = *pulNumPts;

	VARIANT_BOOL vbStatus = VARIANT_FALSE;
	if (pIBinaryImport) 
		vbStatus = pIBinaryImport->GetDataByChannel(sFacID, sStaID, sChanID, dStart, dEnd, &vData);

	// Extract data from the SAFEARRAY and return to Rad as in previous calls
	if ((vbStatus && (vData.vt == (VT_VARIANT | VT_ARRAY)))) 
	{
		long lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vData.parray, 1, &lUBound);

		//pjm 25 Jan 2006 Got rid of compiler warning with explicit cast to long
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

	::VariantClear(&vData); //pjm

	if(vbStatus)
		return 1;
	else
		return 0;
}
*/
///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByStation
//
//	Description:
//	Wraps BinaryCom GetDataByStation method
//	The first instance of the function returns data by the instrument's native format.
//	The second instance of the function returns data by a double format -- used in the
//	analysis functions.  It also separates out the 8 data channels and returns just those
//	values.
//	
//	Declaration:
//	int CBinaryImport::GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_LONG_DATA_PT *pData, unsigned long *pulNumPts, DATE *pdNextTime)
//	int CBinaryImport::GetDataByStation(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct WHOLE_DOUBLE_DATA_PT1 *pData, unsigned long *pulNumPts, DATE *pdNextTime)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			dStart		DATE of interval start to retrieve Binary data records
//			dEnd		DATE of interval end to retrieve Binary data records
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
//	24-Mar-2003	SFK		Added function to change values to double.
//	28-Oct-2004	SFK		Changed to receive all data in the SAFEARRAY
//////////////////////////////////////////////////////////////////
int CBinaryImport::GetDataByStation(
	const short sFacID, 
	const short sStaID, 
	const DATE dStart, 
	const DATE dEnd, 
	struct WHOLE_LONG_DATA_PT *pData,
	//struct WHOLE_LONG_DATA_PT_WITH_DBL_TIMES *pData,
	unsigned long *pulNumPts, 
	DATE *pdNextTime)
{
	*pdNextTime = 0.0;

	if (*pulNumPts == 0) 
		return(1);

	//DATE dActualStartTime, dActualEndTime;
	
	VARIANT vData;
	::VariantInit(&vData);

	VARIANT_BOOL vbStatus = VARIANT_FALSE;
	vData.vt = VT_I4; //pjm
	vData.lVal = *pulNumPts;
	//vbStatus = pglTimeAlign->GetDataByStation(sFacID, sStaID, dStart, dEnd, &vData, BINARY_TYPE, pulNumPts);
	//vbStatus = pIBinaryImport->GetDataStatistics(sFacID,sStaID,-1,dStart,dEnd,&dActualStartTime,&dActualEndTime,pulNumPts);
	vbStatus = pIBinaryImport->GetDataByStation(sFacID, sStaID,dStart,dEnd,&vData);

	if (vbStatus && (vData.vt == (VT_VARIANT | VT_ARRAY))) 
	{
		long i, lUBound;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vData.parray, 1, &lUBound);
		//pjm 25 Jan 2006 Got rid of compiler warning with explicit cast to long
		if (lUBound > (long)*pulNumPts)		// got back more points than expected only return first *pulNumPts
			lUBound = (long)*pulNumPts - 1;
		// the binary COM puts things in the variant array in this order:
		//	status, node number, station number, state, bit and reserved
		//pjm 25 Jan 2006 Got rid of compiler warning with explicit cast to long
		for (i = 0; i < (long)*pulNumPts; i++) 
		{
			SA_GetElem(vData.parray, i, 0, &(pData[i].dJulianTime));
			SA_GetElem(vData.parray, i, 1, (long *)&(pData[i].ulStatus));	// status
			SA_GetElem(vData.parray, i, 2, &(pData[i].lData[0]));			// log node number
			SA_GetElem(vData.parray, i, 3, &(pData[i].lData[1]));			// station number
			SA_GetElem(vData.parray, i, 4, &(pData[i].lData[2]));			// state
			SA_GetElem(vData.parray, i, 5, &(pData[i].lData[3]));			// bit
			SA_GetElem(vData.parray, i, 6, &(pData[i].lData[4]));			// reserved

			// fill in the stuff we don't use
			pData[i].lData[5] = 0;
			pData[i].lData[6] = 0;
			pData[i].lData[7] = 0;
			pData[i].dElapsedTime = 0;
		}
		SA_GetElem(vData.parray, i, 0, pdNextTime);		// timestamp of next data point
	}
	else
	{
		vbStatus = VARIANT_FALSE;
	}

	::VariantClear(&vData);  //pjm

	if (vbStatus)
		return 1;
	else
		return 0;
}

// separate the states in the "8 channels"
// 22-Jun-2005 SFK  Get data out of binary record properly
int CBinaryImport::GetDataByStation(
	const short sFacID, 
	const short sStaID, 
	const DATE dStart, 
	const DATE dEnd, 
	struct WHOLE_DOUBLE_DATA_PT1 *pData, 
	unsigned long *pulNumPts,
	DATE *pdNextTime)
{
	*pdNextTime = 0.0;

	if (*pulNumPts == 0) 
		return 1;
	
	VARIANT vData;
	::VariantInit(&vData);

	int iStatus = 0;
	VARIANT_BOOL vbStatus;

	vData.vt = VT_I4;  //pjm
	vData.lVal = *pulNumPts;

	//iStatus = pglTimeAlign->GetDataByStation(sFacID, sStaID, dStart, dEnd, &vData, BINARY_TYPE, pulNumPts);
	vbStatus = pIBinaryImport->GetDataByStation(sFacID, sStaID,dStart,dEnd,&vData);

	if (vbStatus && (vData.vt == (VT_VARIANT | VT_ARRAY))) 	// extract the data
	{
		long lUBound;
		long i;
		// Determine the Upper Bound of the SAFEARRAY, assumes a zero-based array
		// so the number of members in the SAFEARRAY is lUBound + 1;
		SafeArrayGetUBound(vData.parray, 1, &lUBound);

		//pjm 25 Jan 2006 Got rid of compiler warning with explicit cast to long
		if (lUBound > (long)*pulNumPts)		// got back more points than expected only return first *pulNumPts
			lUBound = (long)*pulNumPts - 1;
		
		for (i=0; i<lUBound; i++) 
		{
			
			//  the binary COM puts things in the variant array in this order:
			//	timestamp (0), status(1), log node number(2), station number(3), state(4), bit(5)
			//	this function "translates" the states associated with each bit
			//	in the state parameter into data components in the general double
			//	analysis type records.

			SA_GetElem(vData.parray, i, 0, &(pData[i].dJulianTime));
			// 22-Jun-2005 SFK  Get data out of binary record properly
			long lState, lBit;
			SA_GetElem(vData.parray, i, 4, &lState);	// state
			SA_GetElem(vData.parray, i, 5, &lBit);		// bit
			for (int j = 1; j<=8; j++) 
			{
				pData[i].dData[j-1] = 0.0;

				if (j == lBit) 
					pData[i].dData[j-1] = (double)lState;
			}
					
			pData[i].dElapsedTime = 1.0f;
		}
		SA_GetElem(vData.parray, i, 0, pdNextTime);		// timestamp of next data point			
	}
	else
	{
		vbStatus = VARIANT_FALSE;
	}

	::VariantClear(&vData);  //pjm

	if (vbStatus)
		return 1;
	else
		return 0;
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDayStatistics
//
//	Description:
//	Wraps BinaryCom GetDayStatistics BinaryCom method.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CBinaryImport::GetDayStatistics(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, DATE *pdActualStart, DATE *pdActualEnd, unsigned long *pulNumPts)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			dStart		DATE of interval start to retrieve Binary data records
//			dEnd		DATE of interval end to retrieve Binary data records
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
/*
int CBinaryImport::GetDayStatistics(
	const short sFacID, 
	const short sStaID, 
	const DATE dStart, 
	const DATE dEnd, 
	DATE *pdActualStart, 
	DATE *pdActualEnd, 
	unsigned long *pulNumPts)
{
	long lNumPts;
	VARIANT_BOOL vb = VARIANT_FALSE;

	*pdActualStart = 0;
	*pdActualEnd = 0;
	*pulNumPts = 0;

	if (pIBinaryImport) 
		vb = pIBinaryImport->GetDayStatistics(sFacID, sStaID, dStart, dEnd, pdActualStart, pdActualEnd, &lNumPts);

	// convert timestamps back to unsigned longs for now
	if (vb) 
		*pulNumPts = lNumPts;

	return(vb != VARIANT_FALSE);
}
*/

int CBinaryImport::GetDayStatistics(
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

	if (pIBinaryImport == NULL)
		return 0;

	long lNumPts;
	VARIANT_BOOL vb = VARIANT_FALSE;

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID
 
	vb = pIBinaryImport->GetDayStatistics(sFacID, pStaID->sStaID, dStart, dEnd, pdActualStart, pdActualEnd, &lNumPts);

	if (vb)
		*pulNumPts = lNumPts;

	return(vb != VARIANT_FALSE);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataByDay
//
//	Description:
//	Wraps BinaryCom GetDataByDay method.
//	Normally TimeAlign version is called.
//	
//	Declaration:
//	int CBinaryImport::GetDataByDay(const short sFacID, const short sStaID, const DATE dStart, const DATE dEnd, struct DayDataStruct *pDayStruct, unsigned long *pulNumDays)
//
//	Input:	sFacID		Facility number
//			sStaID		Station number
//			dStart		DATE of interval start to retrieve Binary data records
//			dEnd		DATE of interval end to retrieve Binary data records
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
int CBinaryImport::GetDataByDay(
	const short sFacID, 
	const struct IDStruct *pStaID, 
	const DATE dStart, 
	const DATE dEnd, 
	struct DayDataStruct *pDayStruct, 
	unsigned long *pulNumDays)
{
	if (pIBinaryImport == NULL)
		return 0;

	VARIANT vDayData;
	VariantInit(&vDayData);

	long lIndex;
	VARIANT_BOOL vb = VARIANT_FALSE;

	if (*pulNumDays == 0) 
		return(1);	

	// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
	// cast to a long on the way into the TimeAlign interface for station ID
	vb = pIBinaryImport->GetDaySummaryData(sFacID, pStaID->sStaID, dStart, dEnd, &vDayData);

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
	return(vb != VARIANT_FALSE);
}

// 28-Oct-2004	SFK	Added sFacilityID arg to calling parms of function and COM call
int CBinaryImport::BackupDatabase(const short sFacilityID, const char *strPath, const bool bQuestion, const bool bPrompt)
{
	VARIANT_BOOL vb = VARIANT_FALSE;
	
	if (pIBinaryImport) 
		vb = pIBinaryImport->BackupDataStore(sFacilityID, strPath, (VARIANT_BOOL)bQuestion, (VARIANT_BOOL)bPrompt);

	return(vb != VARIANT_FALSE);
}

// 28-Oct-2004	SFK	Added sFacilityID arg to calling parms of function and COM call
int CBinaryImport::RestoreDatabase(const short sFacilityID, const char *strPath, const char *strDestPath, const bool bQuestion, const bool bPrompt)
{
	VARIANT_BOOL vb = VARIANT_FALSE;
	
	if (pIBinaryImport) 
		vb = pIBinaryImport->RestoreDataStore(sFacilityID, strPath, (VARIANT_BOOL)bQuestion, (VARIANT_BOOL)bPrompt);
	return((vb != VARIANT_FALSE)?1:0);
}

// 28-Oct-2004	SFK	Added sFacilityID arg to calling parms of function and COM call
void CBinaryImport::GetDataStoreSize(const short sFacilityID, double *pdSizeInBytes)
{
	if (pIBinaryImport) 
		pIBinaryImport->GetDataStoreSize(sFacilityID, pdSizeInBytes);
}


