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
//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: RADEventList.cpp $
// 
// *****************  Version 8  *****************
// User: Longo        Date: 1/12/05    Time: 2:58p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// default xml file named
// 
// *****************  Version 7  *****************
// User: Longo        Date: 1/11/05    Time: 4:03p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// 
// *****************  Version 6  *****************
// User: Longo        Date: 1/07/05    Time: 2:22p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// marking added to the list elements
// 
// *****************  Version 5  *****************
// User: Longo        Date: 1/05/05    Time: 5:27p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// max value is now a type 'double'
// 
// *****************  Version 4  *****************
// User: Longo        Date: 1/04/05    Time: 4:33p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// %d to %ld, %f to %lg. (longs and doubles are used).
// Added 0 length timespan event culling operation.
// 
// *****************  Version 3  *****************
// User: Longo        Date: 12/23/04   Time: 2:59p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// Expanded event fields to 15, added event id, rates etc.
// 
// *****************  Version 2  *****************
// User: Longo        Date: 12/14/04   Time: 10:23a
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// 
// *****************  Version 1  *****************
// User: Longo        Date: 12/14/04   Time: 9:22a
// Created in $/FDMS/FDMS Application/FDMSNET/AppUl
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// This file contains the class implementations of RADEvents and RADEventList
//////////////////////////////////////////////////////////////
#include "Radbuild.h"
//#include "stdafx.h"
#include ".\radEventList.h"
#include ".\radEventsXML.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Macro that calls a COM method returning HRESULT value:
#define HRCALL(a, errmsg) \
do { \
    hr = (a); \
    if (FAILED(hr)) { \
        goto clean; \
    } \
} while (0)


//////////////////////////////////////////////////////////////
// class RADEvent
//////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////
// RADEvent::RADEvent
//
// Initialize local vars, and auto-init the parent COleDateTime
// 
///////////////////////////////////////////////////////
RADEvent::RADEvent(void) : m_bMark(false)
{
	m_SequenceNumber = 0L;
	m_IntegralSum = 0.0;
	m_MaxCountValue = 0.0;

	m_NumberOfDataPoints = 0L;
	m_EventChannel = 0L;
	m_AvgRealsRate = 0.0;
	m_AvgTotalsRate = 0.0;

	int i;
	for (i=0; i < eRADAttMax; i++)
	{
		values[i][0] = '\0';
	}
}

RADEvent::~RADEvent(void)
{
}

///////////////////////////////////////////////////////
// RADEvent::Image
//
// Format a TCHAR with all data field values displayed
//
// Output: PTCHAR
// 
///////////////////////////////////////////////////////
PTCHAR RADEvent::Image()
{
	static TCHAR s[2*MAXRADVALUESTRINGLEN];
	sprintf(s, "\n%ld %s %s %s %lg %lg %c", m_SequenceNumber,
		m_Times[eRADStartTimestamp].Format("%Y/%m/%d %H:%M:%S"),
		m_Times[eRADEndTimestamp].Format("%Y/%m/%d %H:%M:%S"),
		m_Times[eRADMaxTimestamp].Format("%Y/%m/%d %H:%M:%S"),
		m_IntegralSum, m_MaxCountValue, (m_bMark ? '*' : ' ')
		);

	return s;
}

///////////////////////////////////////////////////////
// RADEvent::SetREValue
//
// Attempt to convert BSTR (string) representation of selected
// data field into a valid instance of the associated C++ type
// 
// Input:
//  att - the selected data field (enum)
//	bstr - string representation
//
// Output:
//  bool
///////////////////////////////////////////////////////
bool RADEvent::SetREValue(RADEventField att, BSTR bstr)
{
    HRESULT hr = 0;

    WideCharToMultiByte( CP_ACP, 0, bstr, -1,
        values[att], MAXRADVALUESTRINGLEN, NULL, NULL );

	switch (att)
	{
	case eRADSequenceNum: // a 4 byte integer value
        HRCALL(VarI4FromStr(bstr, 0, LOCALE_NOUSEROVERRIDE,&m_SequenceNumber), "VarI4");
		break;

	case eRADStartTimestamp:
	case eRADEndTimestamp:
	case eRADMaxTimestamp: // convert string representation to COleDateTime values
		m_Times[att].Convert(values[att]);
		break;

	case eRADIntegralSum: // an 8 byte double value
        HRCALL(VarR8FromStr(bstr, 0, LOCALE_NOUSEROVERRIDE,&m_IntegralSum), "VarR8");
		break;

	case eRADMaxValue: // an 8 byte double value
        HRCALL(VarR8FromStr(bstr, 0, LOCALE_NOUSEROVERRIDE,&m_MaxCountValue), "VarR8");
		break;

	case eRADAvgRealsRate: // an 8 byte double value
        HRCALL(VarR8FromStr(bstr, 0, LOCALE_NOUSEROVERRIDE,&m_AvgRealsRate), "VarR8");
		break;

	case eRADAvgTotalsRate: // an 8 byte double value
        HRCALL(VarR8FromStr(bstr, 0, LOCALE_NOUSEROVERRIDE,&m_AvgTotalsRate), "VarR8");
		break;

	case eRADDataPointNum: // a 4 byte integer value
        HRCALL(VarI4FromStr(bstr, 0, LOCALE_NOUSEROVERRIDE,&m_NumberOfDataPoints), "VarI4");
		break;

	case eRADEventChannel: // a 4 byte integer value
        HRCALL(VarI4FromStr(bstr, 0, LOCALE_NOUSEROVERRIDE,&m_EventChannel), "VarI4");
		break;

	case eRADStationName:
	case eRADEventType:
	case eRADEventSubType:
	case eRADEventID:

		// no change to these strings needed
		break;
	}
 clean: 
   if (FAILED(hr))
       return false;
   else
       return true;
}

///////////////////////////////////////////////////////
// RADEvent::SetREValue
//
// Attempt to convert BSTR (string) representation of selected
// data field into a valid instance of the associated C++ type
// 
// Input:
//  att - the selected data field (enum)
//	pstr - TCHAR* string representation
//
// Output:
//  bool
///////////////////////////////////////////////////////
bool RADEvent::SetREValue(RADEventField att, PTCHAR pstr)
{

	strcpy(values[att],pstr);

	switch (att)
	{
	case eRADSequenceNum: // a 4 byte integer value
        sscanf(pstr, "%ld", &m_SequenceNumber);
		break;

	case eRADStartTimestamp:
	case eRADEndTimestamp:
	case eRADMaxTimestamp: // convert string representation to COleDateTime values
		m_Times[att].Convert(pstr);
		break;

	case eRADIntegralSum: // an 8 byte double value
        sscanf(pstr, "%lg", &m_IntegralSum);
		break;

	case eRADAvgRealsRate: // an 8 byte double value
        sscanf(pstr, "%lg", &m_AvgRealsRate);
		break;

	case eRADAvgTotalsRate: // an 8 byte double value
        sscanf(pstr, "%lg", &m_AvgTotalsRate);
		break;

	case eRADMaxValue: // an 8 byte double value
        sscanf(pstr, "%lg", &m_MaxCountValue);
		break;

	case eRADDataPointNum: // a 4 byte integer value
        sscanf(pstr, "%ld", &m_NumberOfDataPoints);
		break;

	case eRADEventChannel: // a 4 byte integer value
        sscanf(pstr, "%ld", &m_EventChannel);
		break;

	case eRADStationName:
	case eRADEventType:
	case eRADEventSubType:
	case eRADEventID:
		// no change to these strings needed
		break;
	}

       return true;
}





//////////////////////////////////////////////////////////////
// class RADEventsList
//////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////
// RADEventsList::Titles
//
// A static array of TCHAR, used for date field/column titles.
// The values here come from RAD Review.
///////////////////////////////////////////////////////
char* RADEventsList::Titles[eRADAttMax] =
{
	"RAD-Sequence Num",
	"RAD-Start Timestamp",
	"RAD-End Timestamp",	
	"RAD-Max Timestamp",	
	"RAD-Integral Sum",	
	"RAD-Max Value",
	"RAD-Max Reals Rate",
	"RAD-Max Totals Rate",
	"Station Name",
	"Station Type",
	"RAD-Event Channel",
	"RAD-Type",
	"RAD-SubType",
	"RAD-Data Point Num",
	"RAD-Event ID"
};



///////////////////////////////////////////////////////
// RADEventsList::RADEventsList
//
// Create XML persistence object on the instance
// 
///////////////////////////////////////////////////////
RADEventsList::RADEventsList(void)
{
	bChanged = false;
	pRADEventsXML = new RADEventsXML();
}

///////////////////////////////////////////////////////
// RADEventsList::~RADEventsList
//
// Delete the XML persistence object on the instance
// Free all RADEvents objects on the CObList
//
///////////////////////////////////////////////////////
RADEventsList::~RADEventsList(void)
{
	if (pRADEventsXML)
		delete 	pRADEventsXML;

	POSITION pos = GetHeadPosition();
	while( pos != NULL )
	{
		RADEvent* p = (RADEvent*)GetNext( pos );
		if (p)
			delete p;
	}
	RemoveAll();
}

///////////////////////////////////////////////////////
// RADEventsList::CullSingletons
//
// Remove any events with a timespan <= 0
//
//
///////////////////////////////////////////////////////
void RADEventsList::CullSingletons()
{
	POSITION pos1, pos2;
	RADEvent* p;
	for( pos1 = GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
	{
		p = (RADEvent*) GetNext( pos1 );
		DATE span = p->m_Times[eRADEndTimestamp] -   p->m_Times[eRADStartTimestamp];
		if( span <= 0.0)
		{
			p =  (RADEvent*) GetAt( pos2 ); // Save the old pointer for deletion.
			RemoveAt( pos2 );
			delete p; // Deletion avoids memory leak.
		}
	}
}

///////////////////////////////////////////////////////
// RADEventsList::CullMarked
//
// Remove any events with a bMark set true
// See the RADEvent class for bMark declaration
//
///////////////////////////////////////////////////////
void RADEventsList::CullMarked()
{
	POSITION pos1, pos2;
	RADEvent* p;
	for( pos1 = GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
	{
		p = (RADEvent*) GetNext( pos1 );
		if(p->m_bMark)
		{
			p =  (RADEvent*) GetAt( pos2 ); // Save the old pointer for deletion.
			RemoveAt( pos2 );
			delete p; // Deletion avoids memory leak.
		}
	}
}

///////////////////////////////////////////////////////
// RADEventsList::SetLocationPath
//
// Set the partial or full file system path for the
// persistent external representation (not including
// the file name).
// Default is ".".
//
// Input:
//	path - path name
//
///////////////////////////////////////////////////////
void RADEventsList::SetLocationPath(PTCHAR pPath)
{
	try
	{
		((RADEventsXML*)pRADEventsXML)->SetLocationPath(pPath);
	}
	catch (...)
	{
	}
}

///////////////////////////////////////////////////////
// RADEventsList::SetFileName
//
// Set the file name for the persistent external representation.
// Default is "radfdms.xml".
//
// Input:
//	fname - file name for the external representation of the list
//
///////////////////////////////////////////////////////
void RADEventsList::SetFileName(PTCHAR pFName)
{
	try
	{
		((RADEventsXML*)pRADEventsXML)->SetFileName(pFName);
	}
	catch (...)
	{
	}
}


///////////////////////////////////////////////////////
// RADEventsList::GetDefaultFilename
//
//	default file name for the input/output file, override with RADEventsList::SetFileName()
///////////////////////////////////////////////////////
PTCHAR RADEventsList::GetDefaultFilename()
{
	return RADEventsXML::szDefaultFilename;
}

///////////////////////////////////////////////////////
// RADEventsList::SetFullFilePath
//
// Set the file name and file path for the persistent external representation.
// Default is "radfdms.xml".
//
// Input:
//	path - full file path <dir>\\<name> name for the external representation of the list
//
///////////////////////////////////////////////////////
void RADEventsList::SetFullFilePath(PTCHAR pPath)
{
	try
	{
		((RADEventsXML*)pRADEventsXML)->SetFullFilePath(pPath);
	}
	catch (...)
	{
	}
}

///////////////////////////////////////////////////////
// RADEventsList::Persist
//
// Use the XML Persistence object to serialize (write to
// disk, in this implementation the RADEventsList as an
// XML document.
//
///////////////////////////////////////////////////////
void RADEventsList::Persist()
{
	try
	{
		((RADEventsXML*)pRADEventsXML)->Persist(*this);
	}
	catch (...)
	{
	}
}

///////////////////////////////////////////////////////
// RADEventsList::Populate
// 
// Use the XML file implementation of the list persistence
// object to populate this RADEventsList with RADEvent objects.
//
///////////////////////////////////////////////////////
void RADEventsList::Populate()
{
	try
	{
		((RADEventsXML*)pRADEventsXML)->Populate(*this);
	}
	catch (...)
	{
	}
}





