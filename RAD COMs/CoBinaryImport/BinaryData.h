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
// BinaryData.h
#ifndef BINARYDATA_H
#define BINARYDATA_H

#include "AccessBaseData.h"

#define NUM_RAW_DATA_PTS_IN_GROUP 100

// Because different types of "instruments" are in an event file, define them here
#define BINARY_INST	0
#define GPS_INS		1

#define INTERPOLATED_FRACTION 1.0/8640000.0		// 1/100th of a second
#define INTERPOLATED_DATA_POINT 0x80000000		// set ms bit if interpolated point


/////////////////////////////////////////////////////////////////////////////
// CBinaryRawData -  each data point written to the Access database is one of these
class CBinaryRawDataPt : public CObject {
public:
	DATE			m_dJulianTime;
	unsigned char	m_ucStatus;			// status bytes for record
	unsigned short	m_usLogNodeNumber;		// node number associated with record
	unsigned short	m_usStationNumber;	// station number associated with record
	unsigned char	m_ucState;			// State  (1 or 0)
	unsigned short	m_usBit;			// Bit addressed in this record
	unsigned char	m_ucReserved;
};


/////////////////////////////////////////////////////////////////////////////
// CBinaryDbData - each data point read from the Access database is one of these
class CBinaryDbDataPt : public CObject {
public:
	//COleDateTime	m_Timestamp;
	DATE	m_dTimestamp;
	int		m_iStatus;
	int		m_iLogNodeNumber;
	int		m_iStationNumber;
	int		m_iState;
	int		m_iBit;
	int		m_iReserved;
};


/////////////////////////////////////////////////////////////////////////////
// CBinaryData - Binary specific versions of what all data objects must do/be.
class CBinaryData : public CBaseAccessData 
{
public:
	// define the columns numbers for the StationXXXBinaryData tables
	#define BIN_DATETIME_COL		0
	#define BIN_TIMESTAMP_COL		1
	#define BIN_STATUS_COL			2
	#define BIN_LOGNODE_NUMBER_COL	3
	#define BIN_STATION_NUMBER_COL	4
	#define BIN_STATE_COL			5
	#define BIN_BIT_CHAN_COL		6
	#define BIN_RESERVED_COL		7
	#define BIN_NUM_COLS			8

	CBinaryData(
		CDbAccess* pDb, 
		const int iStaNum, 
		const int iChanNum, 
		const int iInstType, 
		const bool bInterpolated,
		const bool bQuietMode);

	~CBinaryData();

	bool CreateBinaryTable(CString *pstrErrorMsg);

	bool AddDayData(const DB_D_INTERVAL& ActualDay, unsigned long ulNumPtsInDay, CString *pstrErrorMsg);
	bool DayExists(DATE dTime, unsigned long *pulNumPts, DB_D_INTERVAL *pdActual);
	bool DeleteDay(DATE dTime);
	
	bool GetStationData(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, CString *pstrErrorMsg=NULL);
	bool GetChannelData(const int &iChanOffset, const DB_D_INTERVAL &dRequested, DB_D_INTERVAL* pdActual, DATE *pdTimestamp, double *pdData, CString *pstrErrorMsg=NULL);
	bool GetDataStatistics(const DB_D_INTERVAL &dRequested, DB_D_INTERVAL *pdActual, CString *pstrErrorMsg=NULL);
	bool AddData(CString *pstrErrorMsg=NULL);

	CBinaryRawDataPt* m_pBinaryRawPts;		// points to the array of data points to add to the db
	CBinaryDbDataPt* m_pBinaryDbPts;		// points to the array of data points read from the db

private:
	CString m_strBinColNames[BIN_NUM_COLS];
	COleSafeArray m_vSafeArrayBinColNames;

};
#endif // BINARYDATA_H