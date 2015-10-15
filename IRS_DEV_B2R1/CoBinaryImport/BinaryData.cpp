// BinaryData.cpp: implementation of the CBinaryData class.
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
//	This file contains the specific routines to handle GRAND data
//
//	Functions:
//		CreateBinaryTable
//		AddData
//		GetDataStatistics
//		GetChannelData
//		GetStationData
//		DayExists
//		DeleteDay
//		AddDayData
//		
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BinaryData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define BINARY_DATA_TABLE "Binary Data"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
// The CBinaryData class contains unique functionality associated
// with accessing (storing and retrieving) data from the binary database.
// It implements the pure virtual functions in the CBaseAccessData class.
//////////////////////////////////////////////////////////////////////
CBinaryData::CBinaryData(
	CDbAccess* pDb, 
	const int iStaNum, 
	const int iChanNum, 
	const int iInstType, 
	const bool bInterpolated,
	const bool bQuietMode)
	:CBaseAccessData(pDb, iStaNum, iInstType, bInterpolated,bQuietMode)
{
	//	fill the CString array with the names for the columns in the StationXXXBinaryData tables
	m_strBinColNames[BIN_DATETIME_COL]		= "Date_Time";
	m_strBinColNames[BIN_TIMESTAMP_COL]		= "TheTimestamp";		// don't name this Timestamp as can't index it!
	m_strBinColNames[BIN_STATUS_COL]		= "Status";
	m_strBinColNames[BIN_LOGNODE_NUMBER_COL]= "Log Node Number";
	m_strBinColNames[BIN_STATION_NUMBER_COL]= "Station Number";
	m_strBinColNames[BIN_STATE_COL]			= "State";
	m_strBinColNames[BIN_BIT_CHAN_COL]		= "Bit";
	m_strBinColNames[BIN_RESERVED_COL]		= "Reserved";

	// fill the SafeArray with the names for the columns in the StationXXXBinaryData tables -- 
	// this is used in the AddData routine when we try the faster method of writing records
	long lDimension[1];
	m_vSafeArrayBinColNames.CreateOneDim(VT_VARIANT, BIN_NUM_COLS);
	lDimension[0] = BIN_DATETIME_COL;
	m_vSafeArrayBinColNames.PutElement(lDimension, &(_variant_t(m_strBinColNames[BIN_DATETIME_COL])));

	lDimension[0] = BIN_TIMESTAMP_COL;
	m_vSafeArrayBinColNames.PutElement(lDimension, &(_variant_t(m_strBinColNames[BIN_TIMESTAMP_COL])));

	lDimension[0] = BIN_STATUS_COL;	
	m_vSafeArrayBinColNames.PutElement(lDimension, &(_variant_t(m_strBinColNames[BIN_STATUS_COL])));

	lDimension[0] = BIN_LOGNODE_NUMBER_COL;
	m_vSafeArrayBinColNames.PutElement(lDimension, &(_variant_t(m_strBinColNames[BIN_LOGNODE_NUMBER_COL])));

	lDimension[0] = BIN_STATION_NUMBER_COL;
	m_vSafeArrayBinColNames.PutElement(lDimension, &(_variant_t(m_strBinColNames[BIN_STATION_NUMBER_COL])));

	lDimension[0] = BIN_STATE_COL;
	m_vSafeArrayBinColNames.PutElement(lDimension, &(_variant_t(m_strBinColNames[BIN_STATE_COL])));

	lDimension[0] = BIN_BIT_CHAN_COL;
	m_vSafeArrayBinColNames.PutElement(lDimension, &(_variant_t(m_strBinColNames[BIN_BIT_CHAN_COL])));

	lDimension[0] = BIN_RESERVED_COL;
	m_vSafeArrayBinColNames.PutElement(lDimension, &(_variant_t(m_strBinColNames[BIN_RESERVED_COL])));

	m_pBinaryDbPts = NULL;
	m_iChanNum = iChanNum;
}


CBinaryData::~CBinaryData()
{
	if (m_pBinaryDbPts) delete [] m_pBinaryDbPts;
}


//////////////////////////////////////////////////////////////////////////////////
//	Name:	CreateBinaryTable
//
//	Description:
//	Create the appropriate table to hold the binary data based on the instrument type.
//	There is a separate table for the various instrument types and each station
//	has its own table named by the convention StationXXXBinaryData for binary instruments.
//	XXX is the station id from Facility Cfg COM.
//	The binary instruments have these fields:
//		Date_Time		- date/time associated with data record in "readable" format
//		TheTimestamp	- date/time in DATE format
//		Status			- unused at this time
//		Log Node Number - at which node this information was logged (all ILON numbers have 1000 added to them)
//		Station Number	- which ILON the data came from (all ILON numbers have 1000 added to them)
//		State			- whether the trigger is active or not
//		Bit				- which bit(s) we are talking about
//		Reserved		- further expansion may go here
//
//	Declaration:
//	bool CBinaryData::CreateBinaryTable(CString *pstrErrorMsg)
//
//	Input:	
//			m_iStaNum			which station data associated with
//			m_iInstType			which type of instrument at that station
//			m_pDb				pointer to Access db that data is to be written to
//			m_strBinColNames	contains the names for the columns in the table
//			
//	Output:	pstrErrorMsg	error msg from problem, if any
//	
//	Return:	true (table created or exists) / false (could not create table)
//	
// Date    /	author	revision
// -----------------	--------
// 22-Oct-2002	SFK 	Created from CreateTable in DbBinary.cpp in Rad 3.0
//////////////////////////////////////////////////////////////////////////////////
bool CBinaryData::CreateBinaryTable(CString *pstrErrorMsg)
{
	CString strQuery;
	CString strTemp;
	CString strTableName;
	bool bSuccess = false;

	if (pstrErrorMsg) pstrErrorMsg->Empty();

	if (m_iInstType == BINARY_INST) {
		// for readability and debug create two forms of the timestamp
		strTableName.Format(BINARY_DATA_TABLE);

		strQuery.Format("CREATE TABLE [%s] (", strTableName),
		strTemp.Format("[%s] DATETIME, ", m_strBinColNames[BIN_DATETIME_COL]);
		strQuery += strTemp;
		strTemp.Format("[%s] DOUBLE, ",   m_strBinColNames[BIN_TIMESTAMP_COL]);
		strQuery += strTemp;
		strTemp.Format("[%s] BYTE, ",     m_strBinColNames[BIN_STATUS_COL]);
		strQuery += strTemp;
		strTemp.Format("[%s] SHORT, ",    m_strBinColNames[BIN_LOGNODE_NUMBER_COL]);
		strQuery += strTemp;
		strTemp.Format("[%s] SHORT, ",    m_strBinColNames[BIN_STATION_NUMBER_COL]);
		strQuery += strTemp;
		strTemp.Format("[%s] SHORT, ",    m_strBinColNames[BIN_STATE_COL]);
		strQuery += strTemp;
		strTemp.Format("[%s] SHORT, ",    m_strBinColNames[BIN_BIT_CHAN_COL]);
		strQuery += strTemp;
		strTemp.Format("[%s] BYTE",       m_strBinColNames[BIN_RESERVED_COL]);
		strQuery += strTemp;
		strTemp.Format(");");
		strQuery += strTemp;
		bSuccess = m_pDb->ExecuteSqlCommand (strQuery);

		// for speed in accessing data, create an index
		if (bSuccess) {
			strQuery.Format("CREATE INDEX MyTimeIndex ON [%s] (%s);",
							strTableName, m_strBinColNames[BIN_TIMESTAMP_COL]);
			bSuccess = m_pDb->ExecuteSqlCommand (strQuery);
		}
	}

	if (!bSuccess) if (pstrErrorMsg) pstrErrorMsg->Format("Database error, database is probably corrupted.");
	return(bSuccess);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	AddData
//
//	Description:
//	Add several data records to the database.  This routine assumes all bookkeeping
//	such as deleting existing potentially conflicting records has been done prior to
//	calling this routine.
//	
//	Declaration:
//	bool CBinaryData::AddData(CString *pstrErrorMsg)
//
//	Input:	
//			m_iStaNum			which station data associated with
//			m_iInstType			which type of instrument at that station
//			m_pDb				pointer to Access db that data is to be written to
//			m_ulNumPtsRequested	number of points to be written to db
//			m_pBinaryRawPts		points to an array of m_ulNumPtsRequested points of type CBinaryRawData
//								to be written to the m_pDb database
//			
//			
//	Output:	pstrErrorMsg		error msg from problem, if any
//			m_ulNumPtsActual	number of data points written to the database
//	
//
//	Return:	true (data successfully written) / false (some kind of error see pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	22-Oct-2002	SFK		Created from AddData in DbBinary.cpp in Rad 3.0
//////////////////////////////////////////////////////////////////
bool CBinaryData::AddData(CString *pstrErrorMsg)
{

	bool bTableExists = true;
	bool bSuccess = false;
	COleDateTime DateTime;
	ADODB::_RecordsetPtr pRecordSet;
	bool bWasOpen;
	CString strValue;
	CString strTemp;
	CString strTableName;

	if (pstrErrorMsg) pstrErrorMsg->Empty();

	// check type of instrument and do appropriate write to db
	if (m_iInstType == BINARY_INST) {
		strTableName.Format(BINARY_DATA_TABLE);

		// create the table if it doesn't exist
		if (!DoesTableExist(strTableName)) {
			bTableExists = CreateBinaryTable(pstrErrorMsg);
		}

		if (bTableExists) {
			// for fast write, will create a recordset we can update
			CString strSqlQuery;
			strSqlQuery.Format("SELECT * FROM [%s] WHERE [%s] IS NULL", strTableName, m_strBinColNames[BIN_TIMESTAMP_COL]);
			if (m_pDb->CreateWritableRecordSet(strSqlQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen)) {

				COleSafeArray vSafeArrayValues;
				vSafeArrayValues.CreateOneDim(VT_VARIANT, BIN_NUM_COLS);
				m_ulNumPtsActual = 0;
				long lDimension[1];
				unsigned int ui = 0;
				// now one by one put the new values in our writeable record set and add to the database
				for (ui=0; ui<m_ulNumPtsRequested; ui++) {
					lDimension[0] = BIN_DATETIME_COL;
					vSafeArrayValues.PutElement(lDimension, &(_variant_t(m_pBinaryRawPts[ui].m_dJulianTime)));

					lDimension[0] = BIN_TIMESTAMP_COL;
					vSafeArrayValues.PutElement(lDimension, &(_variant_t(m_pBinaryRawPts[ui].m_dJulianTime)));

					lDimension[0] = BIN_STATUS_COL;
					vSafeArrayValues.PutElement(lDimension, &(_variant_t(m_pBinaryRawPts[ui].m_ucStatus)));

					lDimension[0] = BIN_LOGNODE_NUMBER_COL;
					vSafeArrayValues.PutElement(lDimension, &(_variant_t((short)m_pBinaryRawPts[ui].m_usLogNodeNumber)));

					lDimension[0] = BIN_STATION_NUMBER_COL;
					vSafeArrayValues.PutElement(lDimension, &(_variant_t((short)m_pBinaryRawPts[ui].m_usStationNumber)));

					lDimension[0] = BIN_STATE_COL;
					vSafeArrayValues.PutElement(lDimension, &(_variant_t(m_pBinaryRawPts[ui].m_ucState)));

					lDimension[0] = BIN_BIT_CHAN_COL;
					vSafeArrayValues.PutElement(lDimension, &(_variant_t((short)m_pBinaryRawPts[ui].m_usBit)));

					lDimension[0] = BIN_RESERVED_COL;
					vSafeArrayValues.PutElement(lDimension, &(_variant_t(m_pBinaryRawPts[ui].m_ucReserved)));

					bSuccess = m_pDb->AddRecordSet(pRecordSet, m_vSafeArrayBinColNames, vSafeArrayValues);
					if (!bSuccess) break;

				}
				if (ui > 0) m_ulNumPtsActual = ui-1; // number of points actually written to db
				m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen);
			}
		}
	}
	if (!bSuccess) if (pstrErrorMsg) pstrErrorMsg->Format("Database error, database is probably corrupted.");
	return(bSuccess);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetDataStatistics
//
//	Description:
//	Find the requested interval in the db.  Calculate the number of points
//	in the database for the specified channel, station and facility during 
//	the requested interval (including endpoints).
//	
//	Declaration:
//	bool CBinaryData::GetDataStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrorMsg)
//
//	Input:	dRequested			interval to search for in the database
//			m_iStaNum			which station data associated with
//			m_iChanNum			which channel we are interested in (can be 1 to 16, -1 means any channel)
//			m_iInstType			which type of instrument at that station
//			m_pDb				pointer to Access db that data is to be written to
//			m_bInterpolated		if true, then for each point in the db a "fake" one is created when data is read out
//								needed to provide "edges" on graphed channel data
//			
//	Output:	pstrErrorMsg		error msg from problem, if any
//			m_ulNumPtsActual	number of data points (+ interpolated points) in the database
//	
//
//	Return:	true (data successfully written) / false (some kind of error see pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	22-Oct-2002	SFK		Created from GetDataStatistics in DbBinary.cpp in Rad 3.0
//////////////////////////////////////////////////////////////////
bool CBinaryData::GetDataStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrorMsg)
{

	bool bSuccess = false;
	bool bEmptyIsSuccess = true;
	bool bWasOpen;
	ADODB::_RecordsetPtr pRecordSet;
	unsigned int uiNum;
	CString strTemp, strTemp1, strTableName;
	
	if (pstrErrorMsg) pstrErrorMsg->Empty();
	m_ulNumPtsActual = 0;
	pdActual->dStart = dRequested.dStart;
	pdActual->dEnd = dRequested.dEnd;

	if (m_iInstType == BINARY_INST) {
		strTableName.Format(BINARY_DATA_TABLE);
		// check if the table for the station even exists
		if (!DoesTableExist(strTableName)) return(bSuccess);

		CString strQuery;

		// create query based on whether know which mask bit
		// for unknown reason, the BETWEEN statement sometimes misses the end points unless we
		// add/subtract 1/1000th sec from them -- this was chosen to represent a time unit smaller
		// than we ever expect from our data
		if (m_iChanNum == -1)  {			// want all data points for any mask bit
			strQuery.Format("SELECT [%s] FROM [%s] WHERE [%s] = %d AND [%s] BETWEEN %16.10lf AND %16.10lf ORDER BY [%s];",
							m_strBinColNames[BIN_TIMESTAMP_COL], strTableName, 
							m_strBinColNames[BIN_STATION_NUMBER_COL], m_iStaNum,
							m_strBinColNames[BIN_TIMESTAMP_COL], dRequested.dStart - ONE_THOU_SEC, dRequested.dEnd + ONE_THOU_SEC,
							m_strBinColNames[BIN_TIMESTAMP_COL]);
			bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);
		}
		else {								// want only data points associated with a single mask bit
			strQuery.Format("SELECT [%s] FROM [%s] WHERE [%s] = %d AND [%s] = %d AND [%s] BETWEEN %16.10lf AND %16.10lf ORDER BY [%s];",
						m_strBinColNames[BIN_TIMESTAMP_COL], strTableName, 
						m_strBinColNames[BIN_STATION_NUMBER_COL], m_iStaNum,
						m_strBinColNames[BIN_BIT_CHAN_COL], m_iChanNum, 
						m_strBinColNames[BIN_TIMESTAMP_COL], dRequested.dStart - ONE_THOU_SEC, dRequested.dEnd + ONE_THOU_SEC,
						m_strBinColNames[BIN_TIMESTAMP_COL]);
			bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);
		}

		// Set the parameters describing the data just extracted from the db
		if (bSuccess) {		// got some record from db
			if (uiNum > 0) {
				m_ulNumPtsActual = uiNum;
				if (m_bInterpolated == true) m_ulNumPtsActual += m_ulNumPtsActual;		// double the points if interpolated
				// now read the timestamp of the first and last records in the recordset to return
				bSuccess = m_pDb->GetRecordFieldDouble(pRecordSet, m_strBinColNames[BIN_TIMESTAMP_COL], &pdActual->dStart);
				if (bSuccess) {
					pRecordSet->Move(uiNum-1);
					bSuccess = m_pDb->GetRecordFieldDouble(pRecordSet, m_strBinColNames[BIN_TIMESTAMP_COL], &pdActual->dEnd);
				}
				// need to handle round off -- so make the starting time 1/100th second earlier
				// and ending time 1/100th second later
//				if (bSuccess) {
//					pdActual->dStart = pdActual->dStart - ONE_HUNDREDTH_SEC;
//					pdActual->dEnd = pdActual->dEnd + ONE_HUNDREDTH_SEC;
//				}
			}
			else {
				CreateTimeIntervalString(dRequested, &strTemp);
				if (pstrErrorMsg) pstrErrorMsg->Format("\nNo data found in interval %s ",  strTemp);
			}
			m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen); 	
		}
	}

	if (!bSuccess) if (pstrErrorMsg) pstrErrorMsg->Format("Database error, database is probably corrupted.");
	return(bSuccess);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetChannelData
//
//	Description:
//	Read only x/y data associated with a time interval in the database where x
//	is the timestamp and y is either 1 or 0 for the channel data depending on
//	whether or not the specified bit is set in the TriggerState field.
//	It returns all the data records matching conditions that fall within the
//	interval specified (up to m_ulNumPtsRequested points).  If the data are to be 
//	interpolated (provides "edges" for graph), then for each data point read
//	from the data base, a "fake" one is created with a time slightly
//	before the current point with a value opposite the one read from the db.
//	
//	Declaration:
//	bool CBinaryData::GetChannelData(const int &iChanOffset, const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, DATE *pdTimestamp, double *pdData, CString *pstrErrorMsg)
//
//	Input:	dRequested			interval to search for in the database
//			m_iStaNum			which station data associated with
//			m_iChanNum			which channel we are interested in (can be 1 to 16, -1 means any channel)
//			m_iInstType			which type of instrument at that station
//			m_pDb				pointer to Access db that data is to be written to
//			m_bInterpolated		if true, then for each point in the db a "fake" one is created when data is read out
//								needed to provide "edges" on graphed channel data
//			
//	Output:	pdAcutal			acutal interval the data was retrieved from
//			pstrErrorMsg		error msg from problem, if any
//			m_ulNumPtsActual	number of data points (+ interpolated points) in the database
//			m_dTimestampOfNextData where to start the search for the data following this interval
//	
//
//	Return:	true (data successfully written) / false (some kind of error see pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	22-Oct-2002	SFK		Created from GetChannelData in DbBinary.cpp in Rad 3.0
//////////////////////////////////////////////////////////////////
bool CBinaryData::GetChannelData(const int &iChanOffset, const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, DATE *pdTimestamp, double *pdData, CString *pstrErrorMsg)
{
	bool bSuccess = false;
	bool bEmptyIsSuccess = true;
	bool bWasOpen;
	ADODB::_RecordsetPtr pRecordSet;
	unsigned int uiNum;
	CString strTableName, strTemp;
	
	if (pstrErrorMsg) pstrErrorMsg->Empty();
	m_ulNumPtsActual = 0;
	m_dTimestampOfNextData = dRequested.dEnd;
	pdActual->dStart = dRequested.dStart;
	pdActual->dEnd = dRequested.dEnd;
	if (m_ulNumPtsRequested == 0) return (true);		// didn't request any data
	if ((pdTimestamp == NULL) || (pdData == NULL)) return(false);  // no storage allocated by caller

	if (m_iInstType == BINARY_INST) {
		strTableName.Format(BINARY_DATA_TABLE);
		// check if the table for the station even exists
		if (!DoesTableExist(strTableName)) return(true);	// no table, are finished

		CString strQuery;
		
		// read all the valid records from the db
		// for unknown reason, the BETWEEN statement sometimes misses the end points unless we
		// add/subtract 1/1000th sec from them -- this was chosen to represent a time unit smaller
		// than we ever expect from our data
		strQuery.Format("SELECT [%s], [%s]  FROM [%s] WHERE [%s] = %d AND [%s] = %d AND [%s] BETWEEN %16.10lf AND %16.10lf ORDER BY [%s];",
						m_strBinColNames[BIN_TIMESTAMP_COL], m_strBinColNames[BIN_STATE_COL], strTableName,
						m_strBinColNames[BIN_STATION_NUMBER_COL], m_iStaNum,
						m_strBinColNames[BIN_BIT_CHAN_COL], m_iChanNum, 
						m_strBinColNames[BIN_TIMESTAMP_COL], dRequested.dStart - ONE_THOU_SEC, dRequested.dEnd + ONE_THOU_SEC,
						m_strBinColNames[BIN_TIMESTAMP_COL]);
		bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);
		
		// got the timestamp and state, now put this value and the "fake interpolated" value in the x-y array
		if (bSuccess) {
			if (uiNum > 0) {		// only move the data if we have the amount
				int iState;
				DATE dTimestamp;
				unsigned long ulPtsThisRead = (unsigned long)uiNum;
				// The above read returns all the points in the interval -- not just m_ulNumPtsRequested
				//	that we are looking for.  So now make sure we terminate when we've read as many as
				//	storage has been allocate for.
				if (m_ulNumPtsRequested < ulPtsThisRead) ulPtsThisRead = m_ulNumPtsRequested;
				if (m_bInterpolated == true) ulPtsThisRead += ulPtsThisRead;		// will get two for each data point

				for (ULONG ul=0; ul<ulPtsThisRead; ul++) {  // see DR 523
					// Get the next timestamp and data from the records read from the db
					bSuccess = m_pDb->GetRecordFieldDouble(pRecordSet, m_strBinColNames[BIN_TIMESTAMP_COL], &dTimestamp);
					if (bSuccess) bSuccess = m_pDb->GetRecordFieldShort(pRecordSet, m_strBinColNames[BIN_STATE_COL], &iState);

					if (m_bInterpolated == true) {		// have to create the "fake" point prior to each data point
						if (iState == 0) {		// make the sharp edge to go from 1 to 0
							pdTimestamp[ul] = dTimestamp;
							dTimestamp += INTERPOLATED_FRACTION;
							pdData[ul] = 1;
						}
						if (iState == 1) {		// make the sharp edge to go from 0 to 1
							pdTimestamp[ul] = dTimestamp;
							dTimestamp += INTERPOLATED_FRACTION;
							pdData[ul] = 0;
						}
						ul++;
						m_ulNumPtsActual++;						// count the interpolated point
					}
					pdTimestamp[ul] = dTimestamp;
					pdData[ul] = iState;
					m_ulNumPtsActual++;						// count the point read
					pRecordSet->MoveNext();	
					// and move on to the next point in the recordset

					///////////////////////////////////////////////////////////////////////////////////////
					//
					// DR 523
					//
					// Heather was creating a special one-use version of CoBinaryImport 
					// for the RFID experiment when she came across a crash when RAD attempted
					// to delete an array that it passed into the BinaryImport component.
					// She completed the analysis on September 19, 2007.  She replaced (i.e., worked around)
					// RAD's calls
					// to TimeAlign and subsequently ran across this stomp which blew up when she tried to
					// delete the callers arrays on the RAD client side.
					//
					// The problems are several. 
					//
					// First of all the TimeAlign logic is inserted EVERYWHERE
					// on the RAD client side and is and difficult to reverse engineer.  
					// This is particularly undesireable because time align functionality is not
					// needed in those cases when time aligning is not required (which is most of the cases.)
					// Thus, in order to support the RFID demonstration, Heather coded the TimeAlign functionality out
					// in the RAD client.  This is a blatent hack, but acceptable for a demonstration.
					// 
					// Second, the BinaryImport server component manipulates the raw data, which is inconsistent with
					// the other import components.   The BinaryImport component
					// inserts "interpolated" data (the opposite logic of the REAL data) into the raw data stream for the
					// consumption of the caller.  This is inconsistent with the other import components which always
					// deliver the raw data to the caller client, leaving it to the client to manipulte
					// the data as appropriate to the data type.  The RAD graphic logic for binary data
					// needs to be the place where the opposite-sense (aka "interpolated") data are generated for graphing, 
					// not the import component.
					//
					// Third, the "for (ULONG ul=0; ul<ulPtsThisRead; ul++)" loop above is poorly designed.  The
					// break statement below was intended to keep the "ul" index from going past the
					// end of the array.  It masks an problem caused by tampering with the loop index inside the loop itself.
					// If the loop index were originally incremented with a ul+=2 rather than ul++, then the possibiolity of 
					// a stomp problem would not have arisen in the first place.
					//
					// The BinaryImport component SHOULD deliver raw data just as all of the other import components do.
					// The interpolated points should be inserted into the raw data by the RAD graphics code.  The interface
					// specification should be changed for the BinaryImport component to be consistent with the other
					// import interfaces.  A COM-compliant generic interface would have avoided this entire issue since all
					// of the import components would have logically adhered to the generic specification and would
					// have been alike by definition.
					//
					// We must test this in B2R1 since there may be code in TimeAlign that actually modifies
					// something in order to avoid the stomp caused by the original code here.
					//
					// pjm 19 September 2007
					//
					//if (ul >= m_ulNumPtsRequested) break;	// watch out so you don't write more points than they allocated
					if (ul+2 >= m_ulNumPtsRequested) break;	// watch out so you don't write more points than they allocated
					//
					///////////////////////////////////////////////////////////////////////////////////////

				}

				// now need the next point in the database -- don't know exactly how to do this
				// so fudge by making it just past the last point
				m_dTimestampOfNextData = dTimestamp + INTERPOLATED_FRACTION;
			}
			else {
				CreateTimeIntervalString(dRequested, &strTemp);
				if (pstrErrorMsg) pstrErrorMsg->Format("\nNo data found in interval %s ",  strTemp);
			}
			m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen); 	
		}
	}

	if (!bSuccess) if (pstrErrorMsg) pstrErrorMsg->Format("Database error, database is probably corrupted.");

	return(bSuccess);
}


///////////////////////////////////////////////////////////////////////////
//	Name:	GetStationData
//
//	Description:
//	Read all data records associated with a time interval in the database
//	
//	Declaration:
//	bool CBinaryData::GetStationData(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, CString *pstrErrorMsg)
//
//	Input:	dRequested		requested interval to retrieve data from
//			
//	Output:	pdActual			actual interval data retrieved from
//			pstrErrorMsg		error msg from problem, if any
//	
//			m_ulNumPtsActual	number of points read starting at pdActual->dStart to pdActual->dEnd
//			m_pBinaryDbPts		ptr to array of m_ulNumPtsActual CBinaryDbDataPt objects
//									(user must delete this after use)
//			m_dTimestampOfNextData where to start the search for the data following this interval
//
//	Return:	true (data successfully retrieved) / false (some kind of error, see miErrorNum and pstrErroMsg)
//	
//  date    /	author	revision
//  -----------------	--------
//	28-Oct-2002	SFK		Created from GetStationData in DbBinary.cpp Rad 3.0
//////////////////////////////////////////////////////////////////
bool CBinaryData::GetStationData(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, CString *pstrErrorMsg)
{
	bool bSuccess = false;
	bool bEmptyIsSuccess = true;
	bool bWasOpen;
	ADODB::_RecordsetPtr pRecordSet;
	CBinaryDbDataPt* pTemp;
	unsigned int uiNumPts;
	COleDateTime DateTime;
	CString strTemp, strTemp1;

	CString strTableName;

	if (pstrErrorMsg) pstrErrorMsg->Empty();
	
	m_ulNumPtsActual = 0;	// number of points read during this db read
	pdActual->dStart = 0.0;
	pdActual->dEnd = 0.0;
	m_dTimestampOfNextData = 0.0;

	if (m_iInstType == BINARY_INST) {
		strTableName.Format(BINARY_DATA_TABLE);
		// check if the table for the station even exists
		if (!DoesTableExist(strTableName)) return(bSuccess);
	
		CString strQuery;

		// read all the valid records from the db in the requested interval
		// for unknown reason, the BETWEEN statement sometimes misses the end points unless we
		// add/subtract 1/1000th sec from them -- this was chosen to represent a time unit smaller
		// than we ever expect from our data
		strQuery.Format("SELECT * FROM [%s] WHERE [%s] = %d AND [%s] >= %16.10lf AND [%s] <= %16.10lf ORDER BY [%s];",
						strTableName,
						m_strBinColNames[BIN_STATION_NUMBER_COL], m_iStaNum,
						m_strBinColNames[BIN_TIMESTAMP_COL], dRequested.dStart - ONE_THOU_SEC, 
						m_strBinColNames[BIN_TIMESTAMP_COL], dRequested.dEnd + ONE_THOU_SEC,
						m_strBinColNames[BIN_TIMESTAMP_COL]);
		bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen, &uiNumPts, bEmptyIsSuccess);

		// move the result of the query to m_pBinaryDbPts array where my program can access it
		m_ulNumPtsActual = 0;
		if (bSuccess) {
			if (uiNumPts > 0) {
				int iTempStatus, iTempBit,iTempState, iTempReserved, iTempLogNodeNum;
				DATE dTempTimestamp;
				if (m_bInterpolated == true) m_ulNumPtsRequested += m_ulNumPtsRequested;	// will create interpolate point for every pt read
				if (m_pBinaryDbPts) delete m_pBinaryDbPts;					// delete any previous data allocated
				m_pBinaryDbPts = new CBinaryDbDataPt[m_ulNumPtsRequested];	// allocate storage for all points in db
				pTemp = m_pBinaryDbPts;
				unsigned long ulPtsThisRead = (unsigned long)uiNumPts;
				// The above read returns all the points in the interval -- not just m_ulNumPtsRequested
				//	that we are looking for.  So now make sure we terminate when we've read as many as
				//	storage has been allocate for.
				if (m_ulNumPtsRequested < ulPtsThisRead) ulPtsThisRead = m_ulNumPtsRequested;
				if (pTemp) {
					for (ULONG ul=0; ul<ulPtsThisRead; ul++) {
						//	read the data point from the db
						bSuccess = m_pDb->GetRecordFieldDouble(pRecordSet,m_strBinColNames[BIN_TIMESTAMP_COL], &dTempTimestamp);
						if (bSuccess) bSuccess = m_pDb->GetRecordFieldShort(pRecordSet, m_strBinColNames[BIN_STATUS_COL], &iTempStatus);
						if (bSuccess) bSuccess = m_pDb->GetRecordFieldShort(pRecordSet, m_strBinColNames[BIN_STATE_COL], &iTempState);
						if (bSuccess) bSuccess = m_pDb->GetRecordFieldShort(pRecordSet, m_strBinColNames[BIN_BIT_CHAN_COL], &iTempBit);
						if (bSuccess) bSuccess = m_pDb->GetRecordFieldShort(pRecordSet, m_strBinColNames[BIN_LOGNODE_NUMBER_COL], &iTempLogNodeNum);
						if (bSuccess) bSuccess = m_pDb->GetRecordFieldShort(pRecordSet, m_strBinColNames[BIN_RESERVED_COL], &iTempReserved);
	
						// never interpolate station data???
						//if (m_bInterpolated == true) { // create the interpolate point now as just before this point and opposite state
						//	pTemp->m_dTimestamp =	dTempTimestamp - INTERPOLATED_FRACTION;
						//	pTemp->m_iStatus =		iTempStatus | INTERPOLATED_DATA_POINT;
						//	pTemp->m_iTriggerMask = iTempTriggerMask;
						//	pTemp->m_iTriggerState = ~iTempTriggerState | iTempTriggerMask;
						//	pTemp->m_iReserved =	iTempReserved;
						//	pTemp++;
						//}

						pTemp->m_dTimestamp		= dTempTimestamp;
						pTemp->m_iStatus		= iTempStatus;
						pTemp->m_iState			= iTempState;
						pTemp->m_iBit			= iTempBit;
						pTemp->m_iStationNumber = m_iStaNum;
						pTemp->m_iLogNodeNumber = iTempLogNodeNum;
						pTemp->m_iReserved		= iTempReserved;
						pTemp++;		// advance to next spot to store data in user array

						m_ulNumPtsActual++;

						pRecordSet->MoveNext();	// advance to next record read from db
					}

					// now need the next point in the database -- don't know exactly how to do this
					// so fudge by making it just past the last point
					m_dTimestampOfNextData = dTempTimestamp + INTERPOLATED_FRACTION;
				}
			}
			else {
				CreateTimeIntervalString(dRequested, &strTemp);
				if (pstrErrorMsg) pstrErrorMsg->Format("\nNo data found in interval %s ",  strTemp);
			}
			m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen); 	


		}
	}
	if (!bSuccess) if (pstrErrorMsg) pstrErrorMsg->Format("Database error, database is probably corrupted.");
	return(bSuccess);
}
// End Func


///////////////////////////////////////////////////////////////////////////
//	Name:	DayExists
//
//	Description:
//	For Binary data, we need a special version of DayExists because we need to 
//	look at the actual data table and see if there is any data where the node
//	number matches m_iStaNum.
//	Determine if there is an entry for m_iStaNum of m_iInstType in the DaysTable
//	for the given day and if yes, return some information about the day (number
//	of data points, time of first and last data points for the node for that day).
//
//	Declaration:
//	bool CBinaryData::DayExists(DATE dTime, unsigned long *pulNumPts, DB_D_INTERVAL *pdActual)
//
//	Input:	dTime				some time in the day we are interested in
//			
//	Output:	pulNumPts			number of points in the database for the specified day, station and instrument type
//			pdActual			timestamps of the first and last data points in the specified day for station and instrument type
//
//	Return:	true (day for station and inst type exists) / false (no entry for given day, station and inst type)
//	
//  date    /	author	revision
//  -----------------	--------
//	22-Oct-2002	SFK		// 22-Oct-2002	SFK		Created
//////////////////////////////////////////////////////////////////
bool CBinaryData::DayExists(DATE dTime, unsigned long *pulNumPts, DB_D_INTERVAL *pdActual)
{
	CString strQuery;
	CString strTemp;
	CString strTableName;
	bool bTableExists = true;
	bool bSuccess = false;
	bool bEmptyIsSuccess = true;
	bool bWasOpen;
	ADODB::_RecordsetPtr pRecordSet;
	unsigned int uiNum;

	//if (pstrErrorMsg) pstrErrorMsg->Empty();
	*pulNumPts = 0;
	pdActual->dStart = 0;
	pdActual->dEnd = 0;
	strTableName = BINARY_DATA_TABLE;

	// check if the table of day information even exists
	if (!DoesTableExist(strTableName)) return(bSuccess);

	//Figure out the start time of the day requested
	DATE dStart = (int)dTime;

	// Get information about any days matching the requested one
	strQuery.Format("SELECT * FROM [%s] WHERE [%s] = %d AND [%s] >= %16.10lf AND [%s] <= %16.10lf ORDER BY [%s];",
					strTableName,
					m_strBinColNames[BIN_LOGNODE_NUMBER_COL], m_iStaNum,
					m_strBinColNames[BIN_TIMESTAMP_COL], dStart - ONE_THOU_SEC, 
					m_strBinColNames[BIN_TIMESTAMP_COL], dStart + 1.0 + ONE_THOU_SEC,
					m_strBinColNames[BIN_TIMESTAMP_COL]);
	bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);
	if (bSuccess) {
		if (uiNum > 0)  {	// search through the records for the day and see if there is one for the requested station and type
			DATE dRecordTime;
			// Have records from the node for the entire day.  Now figure out the
			// values that we need to return.
			*pulNumPts = uiNum;
			for (unsigned int i=0; i<uiNum; i++) {
				if (bSuccess) bSuccess = m_pDb->GetRecordFieldDouble(pRecordSet, m_strBinColNames[BIN_TIMESTAMP_COL], &dRecordTime);
				if (uiNum == 0) {
					pdActual->dStart = dRecordTime;
					pdActual->dEnd = dRecordTime;
				}
				if (pdActual->dStart > dRecordTime) pdActual->dStart = dRecordTime;
				if (pdActual->dEnd < dRecordTime) pdActual->dEnd = dRecordTime;
				break;
			}
			pRecordSet->MoveNext();
		}
		m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen); 	
	}

	if (uiNum == 0) return (false);	// no records for the day exist

	//if (!bSuccess) if (pstrErrorMsg) pstrErrorMsg->Format("Database error, database is probably corrupted.");
	return(bSuccess);

}


///////////////////////////////////////////////////////////////////////////
//	Name:	DeleteDay
//
//	Description:
//	Deletes all the data records in the specified day for the specified station and 
//	instrument type.  Also calls the base class routine to delete any day specific stuff.
//
//	For binary data in *.evt/*.bny files, this is more complicated because one *.evt/*.bny file can 
//	contain data from several stations that have been logged to one *.evt/*.bny file.  So the
//	day deletion process must figure out which stations have data in this *.evt/*.bny and delete
//	the associated days and data for each station.
//	
//	Declaration:
//	bool CBinaryData::DeleteDay(DATE dTime)
//
//	Input:	
//			m_iStaNum			which node the data was logged to
//			m_iInstType			which type of instrument at that station
//			m_pDb				pointer to Access db that data is to be written to
//			
//	Output:	none
//
//	Return:	true (data successfully deleted) / false (could delete day)
//	
//  date    /	author	revision
//  -----------------	--------
//	22-Oct-2002	SFK		Created
//////////////////////////////////////////////////////////////////
bool CBinaryData::DeleteDay(DATE dTime)
{
	bool bSuccess = true;
	CString strQuery;
	
	CString strDataTableName, strDayTableName;

	// check type of instrument and do appropriate write to db
	if (m_iInstType == BINARY_INST) {
		strDataTableName.Format(BINARY_DATA_TABLE);
		strDayTableName.Format(DAYS_TABLE);
		// check if the table for the station even exists
		if (DoesTableExist(strDataTableName) && DoesTableExist(strDayTableName)) {
			// for unknown reason, the DELETE statement sometimes misses the end points unless we
			// add/subtract 1/1000th sec from them -- this was chosen to represent a time unit smaller
			// than we ever expect from our data
			DATE dStart = (int)dTime;
			DATE dEnd = dStart + 1.0;	// end of previous day is before start of next day

			// from the days table figure out which stations have data in this day 
			int iSubSta;		// station that had data logged to m_iStaNum node
			int iTempSta;		// used for temporary storage during base class delete
			bool bEmptyIsSuccess = true;
			bool bWasOpen;
			ADODB::_RecordsetPtr pDaysRecordSet, pDataRecordSet;
			unsigned int uiNum = 0;
			unsigned int uiNum1 = 0;
			// query days table for all stations that have data in the day
			strQuery.Format("SELECT * FROM [%s] WHERE [%s] = %d AND [%s] = %lf;",
							strDayTableName,
							m_strDaysCols[DAYS_INSTRUMENT_TYPE_COL], m_iInstType,
							m_strDaysCols[DAYS_DAY_START_TIMESTAMP_COL], dStart);
			bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pDaysRecordSet, &bWasOpen, &uiNum, bEmptyIsSuccess);

			//	Now delete all records for stations in that day that were logged to the station (logging node)
			//	that we are now deleting.
			for (unsigned int ui = 0; ui<uiNum; ui++) {
				// query to see if this station has data in the day and was logged to the specified logging node
				// that is the "station" we are trying to delete now.
				m_pDb->GetRecordFieldInt(pDaysRecordSet, m_strDaysCols[DAYS_STATION_NUMBER_COL], &iSubSta);
				strQuery.Format("SELECT * FROM [%s] WHERE ([%s] = %d) AND ([%s] = %d)AND ([%s] >= %16.10lf) AND ([%s] < %16.10lf);",
								strDataTableName,
								m_strBinColNames[BIN_STATION_NUMBER_COL], iSubSta,
								m_strBinColNames[BIN_LOGNODE_NUMBER_COL], m_iStaNum,
								m_strBinColNames[BIN_TIMESTAMP_COL], dStart - ONE_THOU_SEC, 
								m_strBinColNames[BIN_TIMESTAMP_COL], dEnd + ONE_THOU_SEC);
				bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pDataRecordSet, &bWasOpen, &uiNum1, bEmptyIsSuccess);
				m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pDataRecordSet, bWasOpen); 	

				if (uiNum1 > 0) {
					// delete data for specified substation from the data table
					strQuery.Format("DELETE FROM [%s] WHERE [%s] = %d AND [%s] >= %16.10lf AND [%s] < %16.10lf;",
								strDataTableName,
								m_strBinColNames[BIN_STATION_NUMBER_COL], iSubSta,
								m_strBinColNames[BIN_TIMESTAMP_COL], dStart - ONE_THOU_SEC, 
								m_strBinColNames[BIN_TIMESTAMP_COL], dEnd + ONE_THOU_SEC);
					bSuccess = m_pDb->ExecuteSqlCommand (strQuery);

					// debug -- see if the data is really gone
					m_pDb->GetRecordFieldInt(pDaysRecordSet, m_strDaysCols[DAYS_STATION_NUMBER_COL], &iSubSta);
					strQuery.Format("SELECT * FROM [%s] WHERE ([%s] = %d) AND ([%s] = %d)AND ([%s] >= %16.10lf) AND ([%s] < %16.10lf);",
								strDataTableName,
								m_strBinColNames[BIN_STATION_NUMBER_COL], iSubSta,
								m_strBinColNames[BIN_LOGNODE_NUMBER_COL], m_iStaNum,
								m_strBinColNames[BIN_TIMESTAMP_COL], dStart - ONE_THOU_SEC, 
								m_strBinColNames[BIN_TIMESTAMP_COL], dEnd + ONE_THOU_SEC);
					bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pDataRecordSet, &bWasOpen, &uiNum1, bEmptyIsSuccess);
					m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pDataRecordSet, bWasOpen); 	
					// end debug


					// now delete data fro this station from the days table -- have to temporarily
					// change the m_iStaNum parameter to be the SubStation number.
					iTempSta = m_iStaNum;
					m_iStaNum = iSubSta;
					if (bSuccess) bSuccess = CBaseAccessData::DeleteDay(dTime);	// call the delete time in the base class to finish deletion
					m_iStaNum = iTempSta;

				}
				pDaysRecordSet->MoveNext();
			}

			m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pDaysRecordSet, bWasOpen); 	
		}
	}
	return(bSuccess);
}



///////////////////////////////////////////////////////////////////////////
//	Name:	AddDayData
//
//	Description:
//	Add a record with information for the existing day, station and instrument
//	type to the DaysDataTable.  If there is existing information for the day, 
//	station and instrument type, replace it with the new stuff.
//	For binary data, this is a special version of the function since we have multiple
//	stations embedded in one binary data file.
//	
//	Declaration:
//	bool CBaseAccessData::AddDayData(const DB_D_INTERVAL& ActualDay, unsigned long ulNumPtsInDay, CString *pstrErrorMsg)
//
//	Input:	ActualDay		Portion of day that data exist for
//			ulNumPtsInDay	Number of data points in the day (uninterpolated??)
//
//			m_iStaNum		station number data is from
//			m_iInstType		which type of instrument at the station
//
//	Output:	pstrErrorMsg	string containing error message, if any
//
//	Return:	true / false
//	
//  date    /	author	revision
//  -----------------	--------
// 22-Oct-2002	SFK		Created
//////////////////////////////////////////////////////////////////
bool CBinaryData::AddDayData(const DB_D_INTERVAL& ActualDay, unsigned long ulNumPtsInDay, CString *pstrErrorMsg)
{
	CString strQuery;
	CString strTemp;
	CString strTableName, strDataTableName;
	bool bTableExists = true;
	bool bSuccess = false;
	bool bEmptyIsSuccess = true;
	bool bWasOpen;
	ADODB::_RecordsetPtr pRecordSet, pStationRecordSet;
	unsigned int uiNumPts = 0;
	unsigned int uiNumRecs = 0;
	DATE dStart, dEnd;

	if (pstrErrorMsg) pstrErrorMsg->Empty();
	strTableName = DAYS_TABLE;
	strDataTableName = BINARY_DATA_TABLE;

	int iTempStationNumber = m_iStaNum;		// remember this because we will temporarily change it
	
	// if the DaysDataTable does not exist, create it now.
	if (!DoesTableExist(strTableName)) {
		bTableExists = CreateDaysTable(pstrErrorMsg);
	}
	if (bTableExists) if (!DoesTableExist(strDataTableName)) return(bSuccess);		// no data in database

	// Next get a list of all stations that are in the big binary table for this day
	strQuery.Format("SELECT DISTINCT [%s] from [%s] WHERE [%s] >= %16.10lf AND [%s] <= %16.10lf;",
					m_strBinColNames[BIN_STATION_NUMBER_COL], strDataTableName,
					m_strBinColNames[BIN_TIMESTAMP_COL], ActualDay.dStart - ONE_THOU_SEC, 
					m_strBinColNames[BIN_TIMESTAMP_COL], ActualDay.dEnd + ONE_THOU_SEC);
	bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pStationRecordSet, &bWasOpen, &uiNumPts, bEmptyIsSuccess);

	// there is some data for the given day in the database.  Now go through the stations
	// one by one and figure out how many data points they have in the given day and make
	// that entry in the days table
	int iStationNum;
	DATE dDayStart = (int)ActualDay.dStart;
	for (unsigned int i=0; i<uiNumPts; i++) {
		bSuccess = m_pDb->GetRecordFieldShort(pStationRecordSet,m_strBinColNames[BIN_STATION_NUMBER_COL], &iStationNum);
		m_iStaNum = iStationNum;
		strQuery.Format("SELECT [%s] from [%s] WHERE ([%s] = %d) AND ([%s] >= %16.10lf) AND ([%s] <= %16.10lf) ORDER BY [%s];",
					m_strBinColNames[BIN_TIMESTAMP_COL], strDataTableName,
					m_strBinColNames[BIN_STATION_NUMBER_COL], m_iStaNum,
					m_strBinColNames[BIN_TIMESTAMP_COL], ActualDay.dStart - ONE_THOU_SEC, 
					m_strBinColNames[BIN_TIMESTAMP_COL], ActualDay.dEnd + ONE_THOU_SEC,
					m_strBinColNames[BIN_TIMESTAMP_COL]);
		bSuccess = m_pDb->CreateRecordSet(strQuery, (ADODB::_RecordsetPtr *)&pRecordSet, &bWasOpen, &uiNumRecs, bEmptyIsSuccess);

		if (uiNumRecs > 0) {
			//	Delete any existing record about this day, station and instrument type in the days table
			bSuccess = CBaseAccessData::DeleteDay(ActualDay.dStart);

			// are rid of any old stuff, now add the new day record
			if (bSuccess) {
				// get the first time in the data set
				bSuccess = m_pDb->GetRecordFieldDouble(pRecordSet,m_strBinColNames[BIN_TIMESTAMP_COL], &dStart);
				// move to the send of the data set
				for (unsigned int j=1; j<uiNumRecs; j++) {
					pRecordSet->MoveNext();
				}
				bSuccess = m_pDb->GetRecordFieldDouble(pRecordSet,m_strBinColNames[BIN_TIMESTAMP_COL], &dEnd);
				// are ready to add this day's record into the Station Days Table
				strQuery.Format("INSERT INTO [%s] ([%s], [%s], [%s], [%s], [%s], [%s])",strTableName,
							m_strDaysCols[DAYS_STATION_NUMBER_COL],
							m_strDaysCols[DAYS_INSTRUMENT_TYPE_COL],
							m_strDaysCols[DAYS_DAY_START_TIMESTAMP_COL],
							m_strDaysCols[DAYS_FIRST_DATA_TIMESTAMP_COL],
							m_strDaysCols[DAYS_LAST_DATA_TIMESTAMP_COL],
							m_strDaysCols[DAYS_NUM_PTS_COL]);
				strTemp.Format( "VALUES (%d, %d, %lf, %lf, %lf, %d ",
							m_iStaNum, m_iInstType,
							dDayStart, dStart, dEnd,
							uiNumRecs);
				strQuery += strTemp;	
				strTemp.Format( ");");			// add closing parenthesis
				strQuery += strTemp;	
				bSuccess = m_pDb->ExecuteSqlCommand(strQuery);
				m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pRecordSet, bWasOpen); 	
			}
		}

		pStationRecordSet->MoveNext();
	}
	m_pDb->DestroyRecordSet((ADODB::_RecordsetPtr)pStationRecordSet, bWasOpen); 	
	if (!bSuccess) if (pstrErrorMsg) pstrErrorMsg->Format("Database error, database is probably corrupted.");
	m_iStaNum = iTempStationNumber;
	return(bSuccess);
}

