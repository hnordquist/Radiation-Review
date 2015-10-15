//RadEventList.h
#pragma once

//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: RADEventList.h $
// 
// *****************  Version 7  *****************
// User: Longo        Date: 1/12/05    Time: 2:58p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// default xml file named
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
// User: Longo        Date: 1/03/05    Time: 10:21a
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// station number, the leading "0000" not used in timestamp string
// 
// *****************  Version 2  *****************
// User: Longo        Date: 12/23/04   Time: 2:59p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// Expanded event fields to 15, added event id, rates etc.
// 
// *****************  Version 1  *****************
// User: Longo        Date: 12/14/04   Time: 9:22a
// Created in $/FDMS/FDMS Application/FDMSNET/AppUl
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// This file contains a class definitions that present a persisent respresentaiton
// of a list of 0 or more RAD Events
//
// The class RADEvent describes a radiation event as defined in the Radiation Review application
//
// The class RADEventList is a list of RADEvent instances, with associated methods implementing
// external persistence of the list of radiation events. Persistence is implemented using a simple
// XML representation of the list and its content. See the class RADEventsXML for details.
//
//////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////
// Typedef RADEventField: an enumerated type representing each data
// field of a RADEvent. This type is used to declare, and
// subsequently index, the array of string representations of a RADEvent.
//////////////////////////////////////////////////////////////
typedef enum {
	eRADSequenceNum,
	eRADStartTimestamp, // Beginning Time
	eRADEndTimestamp,   // Ending Time
	eRADMaxTimestamp,   // Time of maximum count rate

	eRADIntegralSum,  // Integral sum (sum of counts from and including beginning and ending time)
	eRADMaxValue, // Maximum count rate (related to eRADMaxTimestamp)
	eRADAvgRealsRate, // For SR data, average reals rate
	eRADAvgTotalsRate, // For SR data, average totals rate

	eRADStationName, //  Which station the event is associated with
	eRADStationType, // The type of the station

	eRADEventChannel, // Which channel the event is associated with
	eRADEventType, // Type of event (Unknown, Channel, Measurement or Background)
	eRADEventSubType, //  "Subtype" of event (For Measurement is Unknown, Empty, Assay, Cf Check, Cf Normalization ot Totals)  (For Background is Normal Background, Low Background or High Background)
	eRADDataPointNum,// //  Number of data points in the event
	eRADEventID, // Event ID (up to 13 characters).
	
	eRADAttMax
} RADEventField;

#define MAXRADVALUESTRINGLEN 128

#include ".\ts.h"  // The COleTimeStamp conversion class

//////////////////////////////////////////////////////////////
// Class RADEvent
//
// A RADEvent represents the 11 elements of an 'Event' fomr Radarion Review.
// The class maintains a string representation of each item,
// and a typed representation of each item (second column in table below).
//
//	Sequence Number			long
//	Start Timestamp			COleDateTime
//	End Timestamp			COleDateTime
//	Max Timestamp			COleDateTime
//	Integral Sum			double
//	Max Count Value			double
//  Average Reals Rate		double
//  Average Totals Rate		double
//	Station Type			string
//	Station Name			string

//	Event Channel			long
//	Event Type				string
//	Event Subtype			string
//	Event Number of points	long
//	Event ID of points		string

//	The meaning of these items is found in the Radiation Review User Guide
//
/*
	RADEvents object are populated in two ways.
		1.	Individually assigning string values to the array of strings on the
		object. Use this technique when preparing a list of event for export to
		an external file.

		2.	Using the SetREValue method to set individual fields.  Use this
		technique to create RADEvent objects with typed data fields, this
		method assigns the string value and converts the string into the typed
		value on the object.  When constructing a list of events from an
		external file, this method is used to construct each event from its
		external string representation.

	See the sample code in the description of the container class RADEventsList

*/
// Methods:
//	RADEvent()			constructor
//  ~RADEvent(void);		destructor
//	bool SetREValue(RADEventField att, BSTR bstr);  // assigns both string and typed value to the event
//	PTCHAR Image();  // prepares a string representation of the Event Data, good for debugging
//
//////////////////////////////////////////////////////////////
class RADEvent: public CObject
{
public:

	///////////////////////////////////////////////////////
	// Typedef RADEventDateTimes, an enumerated type representing
	// each Date/Time data field of a RADEvent. This is used to
	// declare and index array of all COleDateTime objects on a RADEvent.
	///////////////////////////////////////////////////////
    typedef enum
	{
		eStartTimestamp = eRADStartTimestamp,
		eEndTimestamp = eRADEndTimestamp,
		eMaxTimestamp = eRADMaxTimestamp,
		eREDatesMax
	} RADEventDateTimes;

	// You must understand these 2 class methods.
	// If you do not, stop whatever you are thinking about right now, and go read a C++ book.
	RADEvent(void);
	~RADEvent(void);

	///////////////////////////////////////////////////////
	// SetREValue
	//
	// Set the string repesentation AND the typed representation of s selected data field.
	// 
	// Input:
	//	att	- selected data fields of a RADEvent
	//  bstr - BSTR string rep of an event field value, as obtained from an
	//			external representation of the RADEvent
	// Output:
	//  bool - true indicates successful storage and data conversion occurred, false otherwise
	///////////////////////////////////////////////////////
	bool SetREValue(RADEventField att, BSTR bstr);

	///////////////////////////////////////////////////////
	// SetREValue
	//
	// Set the string repesentation AND the typed representation of s selected data field.
	// 
	// Input:
	//	att	- selected data fields of a RADEvent
	//  pstr - TCHAR* string rep of an event field value
	// Output:
	//  bool - true indicates successful storage and data conversion occurred, false otherwise
	///////////////////////////////////////////////////////
	bool SetREValue(RADEventField att, PTCHAR pstr);

	///////////////////////////////////////////////////////
	// Image
	//
	// For debugging, a text representation of the event fields.
	// 
	///////////////////////////////////////////////////////
	PTCHAR Image();

	// Array of string representations of each data field
	TCHAR values[eRADAttMax][MAXRADVALUESTRINGLEN];

	// Typed values for each RADEvent data field
	long m_SequenceNumber;
	long m_NumberOfDataPoints;
	long m_EventChannel;
	double m_IntegralSum;
	double m_AvgRealsRate;
	double m_AvgTotalsRate;
	double m_MaxCountValue;
	TS m_Times[eREDatesMax];
	bool m_bMark;

};  // RADEvent




//////////////////////////////////////////////////////////////
// Class RADEventsList
//
// A subclass of CObList.
// This class implements a list of RADEvent objects.
// See CObList for the standard list operations.
//
// RADEventsList features specific functionality to transform the
// list of RADEvent objects into a persistent external represenation. And the
// reciprocal behavior is here: recreating a list of RADEvent objects from
// a persistent external represenation.
//
// An additional feature of RADEventsList is the column naming or title array.
// The original Events list was created to exchange data from RAD with other tools, such as INCC.
// A columnar text file was defined, the first row in the file representing the columns and titles.
// The RADEventsList class maintains an array of strings, indexed by the type RADEventField,
// declaring the same column titles. 
//
// Task A: creating a persistent representation of a list of events.
// (See the note at the end of this file for sample C++ code.)
//	Create a RADEventList object
//  Determine a set of 0 or more eents to be persisted.
//  For each event,
//		Create a RADEvent object
//		Populate the 11 fields of the event with associated strings
//		Insert the RADEvent object on the RADEventList
//
//	Optionally specify the path and file name for the persistent external source.
//	Call the Persist() method of the RADEventsList object, creating an in-memory
//     representation of the event list, and eventually writing it out to the external source.
//	Destruct the RADEventsList object when finished.
//
//
// Task B: creating a RADEventsList of RADEvent objects from a persistent external source.
//	Create a RADEventList object
//	Optionally specify the path and file name for the persistent external source.
//	Call the Populate() method. Populate() opens the external source, parses the content,
//	and then for each RAD Event node in the document,
//		create a RADEvent object, appending it to the RADEventsList.
//
//	Use the RADEventsList as desired. (see CObList and RADEvent)
//	Destruct the RADEventsList object when finished.
// 
//
/*
	Public methods:

		RADEventsList(void);
		~RADEventsList(void);

		void Populate();			See Task A above
		void Persist();				See Task B above

		void SetLocationPath(PTCHAR path);		// set path to external persistent location
		void SetFileName(PTCHAR fname);			// set name of external persistent file

		void AddToEnd( RADEvent* newEvent );	// append RADEvent object to END of list
		RADEvent* GetFromFront();				// retrieve first RADEvent object on the list

*/
//
//////////////////////////////////////////////////////////////
class RADEventsList : public CObList
{
public:
    RADEventsList(void);
    ~RADEventsList(void);

public:

	///////////////////////////////////////////////////////
	// Populate
	//
	// Create a list of RADEvent objects from the RADEventsList
	// persistence representation. Currently, the implementation
	// is an external physical XML file. The file is located,
	// opened, parsed and processed to build a list of RADEvent
	// objects on a RADEventsList instance.
	// The persistent representation is subject to change,
	// and may not be a physical file; remote database table,
	// a remote web service, manual user entry: all are possible.
	//
	///////////////////////////////////////////////////////
    void Populate();

	///////////////////////////////////////////////////////
	// Populate
	//
	// Set the current contents of The RADEventsList, 0 or more
	// RADEvent objects, to the persistent stored representation.
	// In practice, this means constructing a single XML document
	// and writing it to a file location.
	//
	///////////////////////////////////////////////////////
    void Persist();

	///////////////////////////////////////////////////////
	// SetLocationPath
	//
	// Set the partial or full file system path for the
	// persistent external representation (not including
	// the file name).
	// Default is ".".
	//
	// Input:
	//	pPath - path name
	//
	///////////////////////////////////////////////////////
    void SetLocationPath(PTCHAR pPath);

	///////////////////////////////////////////////////////
	// SetFileName
	//
	// Set the file name for the persistent external representation.
	// Default is "radfdms.xml".
	//
	// Input:
	//	pFName - file name for the external representation of the list
	//
	///////////////////////////////////////////////////////
	void SetFileName(PTCHAR pFName);

	///////////////////////////////////////////////////////
	// SetFullFilePath
	//
	// Set the file name and file path for the persistent external representation.
	// Default is "radfdms.xml".
	//
	// Input:
	//	pPath - full file path <dir>\\<name> name for the external representation of the list
	//
	///////////////////////////////////////////////////////
	void SetFullFilePath(PTCHAR pPath);


	///////////////////////////////////////////////////////
	// GetDefaultFilename
	//
	//	default file name for the input/output file, override with SetFileName()
	///////////////////////////////////////////////////////
	static PTCHAR GetDefaultFilename();

	///////////////////////////////////////////////////////
	// RADEventsList::CullSingletons
	//
	// Remove any events with a timespan <= 0.
	// Uses eRADEndTimestamp - eRADStartTimestamp values for the check
	//
	///////////////////////////////////////////////////////
	void CullSingletons();

	///////////////////////////////////////////////////////
	// RADEventsList::CullMarked
	//
	// Remove any events with a bMark set true
	// See the RADEvent class for bMark declaration
	//
	///////////////////////////////////////////////////////
	void CullMarked();

	///////////////////////////////////////////////////////
	// AddToEnd
	//
	// Insert a pointer to a RADEvent object at the end of
	// the RADEventsList list.
	//
	// Input:
	//	newEvent - RADEvent object pointer to place at end of list
	//
	///////////////////////////////////////////////////////
    void AddToEnd( RADEvent* pNewEvent )
        { AddTail( pNewEvent ); bChanged = true;}        // End of the queue

	///////////////////////////////////////////////////////
	// GetFromFront
	//
	// Retrieve the first RADEvent object pointer on the list, remove it from
	// the RADEventsList list.
	//
	// Input:
	//	newEvent - RADEvent object pointer to place at end of list
	//
	///////////////////////////////////////////////////////
    RADEvent* GetFromFront()
        { return IsEmpty() ? NULL : (RADEvent*)RemoveHead(); bChanged = true;}

	bool bChanged;			// true when list has been modified (insertion/deletion of a *RADEvent)

	///////////////////////////////////////////////////////
	//
	// A static array of textual column titles as defined
	// in the persistent representation of the list.
	// Initialized with default values as defined in Radiation Review.
	// Replaced by Populate() list construction.
	//
	///////////////////////////////////////////////////////
	static char* Titles[eRADAttMax]; // the column titles

private:
	void* pRADEventsXML;

};  // RADEventsList


//////////////////////////////////////////////////////////////
// Task A: creating a persistent representation of a list of events.
/*
	// create a new RAD Event List
	RADEventsList* pRadEventsList = new RADEventsList();

	// The external location and name of the RAD to FDMS data interchange file are predefined as .\radfdms.xml,
	// The file name and path can be manually set with these two operations

	pRadEventsList->SetLocationPath(".");		// create the external file in the current working directory
	pRadEventsList->SetFileName("jeeves.xml");  // chnage the name of the external file to 'jeeves.xml'

	// create a new event object
	RADEvent* pOneRADEvent = new RADEvent();

	// place string representations of each of the RAD values onto the RADEvent object
	pOneRADEvent->SetREValue(eRADSequenceNum,"0057");
	pOneRADEvent->SetREValue(eRADStartTimestamp,"0000020010826175602000");
	pOneRADEvent->SetREValue(eRADEndTimestamp,"0000020010826175827000");
	pOneRADEvent->SetREValue(eRADMaxTimestamp,"0000020010826175702000");
	
	pOneRADEvent->SetREValue(eRADIntegralSum,"10941.0");
	pOneRADEvent->SetREValue(eRADMaxValue,"84");
	pOneRADEvent->SetREValue(eRADAvgRealsRate,"8101.01");
	pOneRADEvent->SetREValue(eRADAvgTotalsRate,"8928.23l29029");
	
	pOneRADEvent->SetREValue(eRADStationType,"beowulf");
	pOneRADEvent->SetREValue(eRADStationName,L"GRAND 2");

	pOneRADEvent->SetREValue(eRADEventChannel,L"1");
	pOneRADEvent->SetREValue(eRADEventType,"Background");
	pOneRADEvent->SetREValue(eRADEventSubType,L"Normal Background");

	pOneRADEvent->SetREValue(eRADDataPointNum,L"32");
	pOneRADEvent->SetREValue(eRADEventID,L"CV-17");


	// add the RAD Event to the Event List
	pRadEventsList->AddToEnd(pOneRADEvent);

	// create a new event object
	pOneRADEvent = new RADEvent();

	// populate the object with the event info strings
	pOneRADEvent->SetREValue(eRADSequenceNum,"0059");
	pOneRADEvent->SetREValue(eRADStartTimestamp,"0000020010828175612000");
	pOneRADEvent->SetREValue(eRADEndTimestamp,"0000020010828175812000");
	pOneRADEvent->SetREValue(eRADMaxTimestamp,"00000200108281757012000");
	pOneRADEvent->SetREValue(eRADIntegralSum,"55541.0");
	pOneRADEvent->SetREValue(eRADMaxValue,"99");

	pOneRADEvent->SetREValue(eRADAvgRealsRate,"100101.01");
	pOneRADEvent->SetREValue(eRADAvgTotalsRate,"938928.23l29029");
	
	pOneRADEvent->SetREValue(eRADStationType,"beowulf");
	pOneRADEvent->SetREValue(eRADStationName,L"GRAND 2");

	pOneRADEvent->SetREValue(eRADEventChannel,L"1");
	pOneRADEvent->SetREValue(eRADEventType,"Measurement");
	pOneRADEvent->SetREValue(eRADEventSubType,L"Totals");

	pOneRADEvent->SetREValue(eRADDataPointNum,L"32");
	pOneRADEvent->SetREValue(eRADEventID,L"CV-17");;

	// add the RAD Event to the Event List
	pRadEventsList->AddToEnd(pOneRADEvent);

	// Save the 2 element event list to an external location.
	// Persist is the operation that creates (or overwrites the pre-existing) ".\jeeves.xml".
	pRadEventsList->Persist();

	// Delete the RADEvents list object, thelist destructor also deletes each RADEvent object added to the list above
	delete pRadEventsList;
*/
//
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// Task B: creating a RADEventsList of RADEvent objects from a persistent external source.
/*
	RADEventsList* pRADEventsList = new RADEventsList();

	// The external location and name of the RAD to FDMS data interchange file is predefined as .\radfdms.xml,
	// The file name and path can be manually set with these two operations
	pRADEventsList->SetLocationPath(".");
	pRADEventsList->SetFileName("wooster.xml");  // using ".\wooster.xml"

	// Find the file named ".\wooster.xml", open the file, and parse the XML found in the file.
	// For each event foud in the XML, create a RADEvent object and insert it on the RADEventsList
	pRADEventsList->Populate();

	// use the list of events ...
	POSITION pos = pRADEventsList->GetHeadPosition();
	while( pos != NULL )
	{
		RADEvent* pRADEvent = (RADEvent*)pRADEventsList->GetNext( pos );

		_bstr_t bstrStationLongName = pRADEvent->values[eRADStationName];
		DATE dStart = pRADEvent->m_Times[RADEvent::eStartTimestamp];  // get from Event object
		DATE dEnd = pRADEvent->m_Times[RADEvent::eEndTimestamp];	// get from Event object

		you are doing something with the event information here ...

	}

	// free the list
	delete pRADEventsList;

	*/
//
//////////////////////////////////////////////////////////////


