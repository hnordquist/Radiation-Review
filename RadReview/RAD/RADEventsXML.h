//RadEventsXML.h
#pragma once

//////////////////////////////////////////////////////////////////////////
// Copyright 2005, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: RADEventsXML.h $
// 
// *****************  Version 9  *****************
// User: Longo        Date: 1/12/05    Time: 2:58p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// default xml file named
// 
// *****************  Version 8  *****************
// User: Longo        Date: 1/07/05    Time: 3:52p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// interface leak protection
// 
// *****************  Version 7  *****************
// User: Longo        Date: 1/05/05    Time: 5:27p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// copyright year update
// 
// *****************  Version 6  *****************
// User: Longo        Date: 1/03/05    Time: 10:21a
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// station number, the leading "0000" not used in timestamp string
// 
// *****************  Version 5  *****************
// User: Longo        Date: 12/23/04   Time: 2:59p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// Expanded event fields to 15, added event id, rates etc.
// 
// *****************  Version 4  *****************
// User: Longo        Date: 12/14/04   Time: 9:22a
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// 
// *****************  Version 3  *****************
// User: Longo        Date: 12/10/04   Time: 4:44p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// Full file path setter
// 
// *****************  Version 2  *****************
// User: Longo        Date: 12/09/04   Time: 12:55p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// removed use of CString
// 
// *****************  Version 1  *****************
// User: Longo        Date: 10/18/04   Time: 3:28p
// Created in $/FDMS/FDMS Application/FDMSNET/AppUl
// XML implementation of RAD to FDMS exchange file
//////////////////////////////////////////////////////////////////////////
#pragma once


#include ".\radEventList.h"

// include the Com and XML APIs, used to process XML document representation of an event list
#include <comutil.h>
// HN 10/22/2007 TT #532 Change to import all NDAR dlls and exes
// this include is only changed because Heather only has Version 6 locally loaded.

#include "HPDefines.h"
#import <msxml6.dll> no_namespace raw_interfaces_only

#define MAXRADFDMSPATHLEN 275
#define MAXXAPTHLENGTH 128

//////////////////////////////////////////////////////////////
// Class RADEventsXML
//
// This class implements the persistent XML representation of an
// instance of the RADEventsList class.
//
// RADEventsXML features specific functionality to transform the
// list of RADEvent objects on a RADEventsList into an XML document.
// The reciprocal behavior is also available: recreating a list of
// RADEvent objects from an XML document representation.
//
// This class is a transformation class, and is not used to cache
// and access the repreesented data.
//
// An additional feature of RADEventsXML is the column naming or title array.
// The original Events list was created to exchange data from RAD with
// other tools, such as INCC.  A columnar text file was defined, the
// first row in the file representing the columns and titles.
// The RADEventsXML class maintains an array of strings, indexed by the
// type RADEventField, declaring the same column titles. These titles
// are represented in the XML document processed by the class.
//
// The structure of the XML document representation:
//  root
//   eventlist
//	   columns
//		 column 1
//		 ...
//		 column n
//     event 1    
//     event 2
//     ...
//     event n
//
/*
	Public methods:

		RADEvents(void);
		~RADEvents(void);

		void Populate(RADEventsList& rel);  // build RADEventsList from XML document on disk
		void Persist(RADEventsList& rel);	// convert RADEventsList to XML document, save to disk

		void SetLocationPath(PTCHAR path);		// set path to XML file
		void SetFileName(PTCHAR fname);			// set name of XML file

	Private methods used internally:
		bool ConstructXMLDoc(RADEventsList& rel);  // builds the in-memory DOM object only, use Persist() to write to file
		bool ProcessEvents(RADEventsList& rel, bool bExtract);
		IXMLDOMElement* ConstructEventNode(RADEvent* pEvent); // convert RADEvent to XML event node
		IXMLDOMElement* ConstructEventParentNodeTree();			// construct containing XML document
		IXMLDOMElement* ConstructColumnsNode();					// construct the column title node
		void AddWhiteSpaceToNode(BSTR bstrWs,IXMLDOMNode *pNode);  // add printable formatting entities to the xml document
		static void ReportParseError(IXMLDOMDocument *pDom, char *desc);
		static IXMLDOMDocument* DomFromCOM();				// construct initial DOM document
		RADEvent* XMLToEvent(IXMLDOMNode* pEvent);			// construct an event object from an XML event node
*/
//
//////////////////////////////////////////////////////////////
class RADEventsXML
{
public:
	RADEventsXML(void);
	~RADEventsXML(void);

	///////////////////////////////////////////////////////
	// Populate
	//
	// Update the XML document on this object from the external
	// persistent representation (a file). Then, 
	// for each RADEvent object extracted from the current XML document,
	// insert it on the RADEventsList. Note: the list is not
	// emptied or cleared prior to processing.
	//
	// Input/Output:
	//	rel	- the RADEventsList instance that is populated from
	//      an XML document.
	//
	///////////////////////////////////////////////////////
    void Populate(RADEventsList& rel);

	///////////////////////////////////////////////////////
	// Persist
	//
	// Replace the XML document on this object with a new one,
	// using the RADEventsList parameter. Then save the XML
	// document out to it's persistent representation (a file).
	// Note: the current XML document is replaced, not updated,
	// by this operation.
	//
	// Input/Output:
	//	rel	- the RADEventsList instance that is converted to an
	//		XML document and serialized.
	//
	///////////////////////////////////////////////////////
    void Persist(RADEventsList& rel);

	///////////////////////////////////////////////////////
	// SetLocationPath
	//
	// Set the partial or full file system path (not including
	// the file name), for the persistent external XML file location.
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
	// Set the file name for the persistent external XML file.
	// Default is "radfdms.xml"
	//
	// Input:
	//	pFName -	file name for the external representation of the list
	//
	///////////////////////////////////////////////////////
	void SetFileName(PTCHAR pFName);

	///////////////////////////////////////////////////////
	// SetFullFilePath
	//
	// Set the file path and file name for the persistent external XML file.
	//
	// Input:
	//	pPath -	full file path <dir>\\<name> for the external representation of the list
	//
	///////////////////////////////////////////////////////
	void SetFullFilePath(PTCHAR pPath);

	///////////////////////////////////////////////////////
	// szDefaultFilename
	//
	//	default file name for the input/output file, override with SetFileName()
	///////////////////////////////////////////////////////
	static PTCHAR szDefaultFilename;

private:

	///////////////////////////////////////////////////////
	// ConstructXMLDoc
	//
	// Constructs the XML document from the RADEventList 'rel'.
	// Note: the current XML document is replaced, not updated,
	// by this operation.
	//
	// Input/Output:
	//	rel	- the RADEventsList instance that is converted to an
	//		XML document.
	//
	///////////////////////////////////////////////////////
	bool ConstructXMLDoc(RADEventsList& rel);

	///////////////////////////////////////////////////////
	// ProcessEvents
	//
	// Method to process a list of events, either transforming XML
	// into RADEvents, or RADEvents into XML event nodes.
	// Invoked by the Populate and Persist methods.
	//
	// When transforming the RADEvent objects on the list 'rel' into
	// XML Event nodes, a new XML Event node is created, and appended
	// to the XML Events node of the XML document.
	// 
	// When transforming the XML Events node with XML Event nodes, each
	// XML Event node is transformed to a RADEvent object, and the RADEvent
	// object is appended to the RADEventsList 'rel'.
	//
	// Input/Output:
	//	rel	- the RADEventsList instance
	//  bExtract - processing toggle:
	//		When true, run through the XML event nodes, creating
	//		RADEvent objects on 'rel'.
	//		When false, run through the RADEvent objects, creating
	//		XML event nodes on the XML document.
	//
	///////////////////////////////////////////////////////
	bool ProcessEvents(RADEventsList& rel, bool bExtract); 

	///////////////////////////////////////////////////////
	// ConstructEventNode
	//
	// Creates an XMLEvent node from a RADEvent object
	//
	// An example XML node as text:
	//	<Event num="714"
	//		starttime="20010826131023000"
	//		endtime="20010826131528000"
	//		maxtime="20010826131153000"
	//		sum="21793"
	//		maxvalue="84"
	//		mrrate="20011536"
	//		mtrate="174347"
	//		sname="GRAND 2"
	//		stype="stype715"
	//		echan="2"
	//		etype="Channel"
	//		esubtype="sub"
    //      edpcount="767"
	//		eid="CV-730"/>
	//
	// Note: the text attribute values are created from the strings
	// on the RADEvent object.
	// 
	// Input:
	//	pEvent	- the RADEvent instance
	//
	// Returns:
	//	IXMLDOMElement: the constructed Event node,
	//	or a NULL pointer if processing failed.
	//
	///////////////////////////////////////////////////////
    IXMLDOMElement* ConstructEventNode(RADEvent* pEvent);

	///////////////////////////////////////////////////////
	// ConstructEventParentNodeTree
	//
	// Creates an XMLEvent node for the root of the XML document.
	// The root node is named "IR",
	// the Events container node is a child of "IR":
	// <?xml version="1.0" ?>
	//	<IR>
	//		<Events>
	//		</Events>
	//	<IR>
	//
	//
	// Returns:
	//	IXMLDOMElement: the constructed IR/Eevents node,
	//	or a NULL pointer if processing failed.
	//
	///////////////////////////////////////////////////////
    IXMLDOMElement* ConstructEventParentNodeTree();

	///////////////////////////////////////////////////////
	// ConstructColumnsNode
	//
	// Creates an XML Event node for the Columns node.
	// The Columns node is a singular node placed under the <IR/Events>
	// node.
	// <Columns>
	//		<Title>RAD-Sequence Num</Title> 
	//		<Title>RAD-Start Timestamp</Title> 
	//		<Title>RAD-End Timestamp</Title> 
	//		<Title>RAD-Max Timestamp</Title> 
	//		<Title>RAD-Integral Sum</Title> 
	//		<Title>RAD-Max Value</Title> 
	//		<Title>RAD-Max Reals Rate</Title> 
	//		<Title>RAD-Max Totals Rate</Title> 
	//		<Title>Station Name</Title> 
	//		<Title>Station Type</Title> 
	//		<Title>RAD-Event Channel</Title> 
	//		<Title>RAD-Type</Title> 
	//		<Title>RAD-SubType</Title> 
	//		<Title>RAD-Data Point Num</Title> 
	//		<Title>RAD-Event ID</Title> 
	//	</Columns>
	// The original Events list was created to exchange data from RAD with
	// other tools, such as INCC.  A columnar text file was defined, where the
	// first row in the file representing the columns and titles.
	// The RADEventsXML class maintains an array of strings, indexed by the
	// type RADEventField, declaring the same column titles. These titles
	// are represented in the XML document processed by the class.
	//
	// Returns:
	//	IXMLDOMElement: the constructed Columns node,
	//	or a NULL pointer if processing failed.
	//
	///////////////////////////////////////////////////////
	IXMLDOMElement* ConstructColumnsNode();


	///////////////////////////////////////////////////////
	// AddWhiteSpaceToNode
	//
	// Appends a text node, with variable whitespace content, to
	// the pNode parameter. Used for pleasing decoration of text-based
	// XML external output, for readability.
	//
	// Input/Output:
	//	bstrWs - the whitespace text to append to the node, as a BSTR
	//	pNode - the XML Node to decorate
	//
	///////////////////////////////////////////////////////
	void AddWhiteSpaceToNode(BSTR bstrWs,IXMLDOMNode *pNode);

	///////////////////////////////////////////////////////
	// XMLToEvent
	//
	// Populates a RADEvent object with string and typed values
	// from the XML Event node attributes. The RADEvent class declares
	// the string-to-type conversion operations, per field. The XML node
	// attribute values are placed on the RADEvent as strings, without
	// modification from the XML.
	//
	// Input:
	//	pEvent	- the RADEvent instance
	//
	///////////////////////////////////////////////////////
	RADEvent* XMLToEvent(IXMLDOMNode* pEvent);


	///////////////////////////////////////////////////////
	// DomFromCOM
	//
	// Helper function that creates an empty DOM object.
	// Every well-defined XML document is based on a DOM root object
	//
	// Note: This class uses the MSXML 3.0 Document Object Model. 
	//
	// Returns:
	//    new empty IXMLDOMDocument*, (or NULL on failure)
	//
	///////////////////////////////////////////////////////
    void DomFromCOM();

    TCHAR m_path[MAXRADFDMSPATHLEN];				// file location as set
    TCHAR m_fullpath[MAXRADFDMSPATHLEN];			// fully expressed path and file name
    TCHAR m_fname[MAXRADVALUESTRINGLEN];			// file name

    IXMLDOMDocument *m_pXMLDom;  // document root, created in Populate() and Persist()
    IXMLDOMNode *m_pIRNode;  // parent of Events tree, also set in Populate() and Persist()


	///////////////////////////////////////////////////////
	// Attributes
	// The 15 attributes names used on an XML Event node.
	// Each name maps directly to one field of the same or
	// similar name on a RADEvent object, see the RADEventField type.
	///////////////////////////////////////////////////////
	static char* Attributes[eRADAttMax];

    static TCHAR m_xirevspath[MAXXAPTHLENGTH];		// XPath selector for ir events XML node
    static TCHAR m_xirevsevpath[MAXXAPTHLENGTH];	// XPath selector for a single event node
    static TCHAR m_xcolpath[MAXXAPTHLENGTH];		// XPath selector for the single column title node

    static _bstr_t atts[eRADAttMax]; // the 15 attributes as BSTR, all XML operations use BSTR types
    static _bstr_t bstr_wsn ;		// white space for readability
    static _bstr_t bstr_wsnt;
    static _bstr_t bstr_wsntt;

	static bool bInitialized;		// init toggle flag (see below)

	///////////////////////////////////////////////////////
	// InitStatics
	//
	// Initialize the class static variables only once.
	// Toggle on bInitialized
	//
	///////////////////////////////////////////////////////
	static void InitStatics();
};


