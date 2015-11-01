//////////////////////////////////////////////////////////////////////////
// Copyright 2004, The Regents Of the University of California.  This
// software was produced under U.S. Government contract (W-7405-ENG-36) at
// the Los Alamos National Laboratory by the Safeguards Science and
// Technology Group (N-1).  All Rights Reserved.
//////////////////////////////////////////////////////////////////////////
// $History: RADEventsXML.cpp $
// 
// *****************  Version 10  *****************
// User: Longo        Date: 1/12/05    Time: 2:58p
// Updated in $/FDMS/FDMS Application/FDMSNET/AppUl
// default xml file named

//////////////////////////////////////////////////////////////
// This file contains the class implementation of "RADEventsXML"
//
// RADEventsXML reads and writes XML documents that represent RADEventLists
//
// An example of an XML document managed by this class implementation
/*
<?xml version="1.0" ?> 
 <IR>
	<Events>
		<Columns>
			<Title>RAD-Sequence Num</Title>
			<Title>RAD-Start Timestamp</Title>
			<Title>RAD-End Timestamp</Title>
			<Title>RAD-Max Timestamp</Title>
			<Title>RAD-Integral Sum</Title>
			<Title>RAD-Max Value</Title>
			<Title>RAD-Max Reals Rate</Title>
			<Title>RAD-Max Totals Rate</Title>
			<Title>Station Name</Title>
			<Title>Station Type</Title>
			<Title>RAD-Event Channel</Title>
			<Title>RAD-Type</Title>
			<Title>RAD-SubType</Title>
			<Title>RAD-Data Point Num</Title>
			<Title>RAD-Event ID</Title>
		</Columns>
		<Event num="0004" starttime="0000020010826000213000" endtime="0000020010826000453000" maxtime="0000020010826000328000"
			sum="        9049.0 " maxvalue="     68.0 " mrrate="        0.00 " mtrate="        0.00 "
			sname="GRAND 2" stype="grand" echan="  1 " etype="Channel     " esubtype="Unknown     "
			edpcount="   33 " eid="            " />
		<Event num="0005" starttime="0000020010826015105000" endtime="0000020010826015405000" maxtime="0000020010826015239000"
			sum="       13217.0 " maxvalue="     86.6 " mrrate="        0.00 " mtrate="        0.00 "
			sname="GRAND 2" stype="grand" echan="  1 " etype="Channel     " esubtype="Unknown     "
			edpcount="   37 " eid="            " />
	</Events>
  </IR>
*/
//////////////////////////////////////////////////////////////
#include "RadBuild.h"
//#include "stdafx.h"
#include ".\radEventList.h"
#include ".\radEventsXML.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
static bool bDebug = false;
#else
static bool bDebug = false;
#endif

// Macro that calls a COM method returning HRESULT value:
#define HRCALL(a, errmsg) \
do { \
    hr = (a); \
    if (FAILED(hr)) { \
        dprintf( "%s:%d  HRCALL Failed: %s\n  0x%.8x = %s\n", \
                __FILE__, __LINE__, errmsg, hr, #a ); \
        goto clean; \
    } \
} while (0)

///////////////////////////////////////////////////////
//  RADEventsXML::Attributes
// The 15 attributes names used on an XML Event node.
// Each name maps directly to one field of the same or
// similar name on a RADEvent object, see the RADEventField type.
///////////////////////////////////////////////////////
char* RADEventsXML::Attributes[eRADAttMax] =
{
	"num",
	"starttime",
	"endtime",
	"maxtime",
	"sum",
	"maxvalue",
	"mrrate",
	"mtrate",

	"sname",
	"stype",
	
	"echan",
	"etype",
	"esubtype",
	"edpcount",
	"eid",
};

TCHAR RADEventsXML::m_xirevspath[MAXXAPTHLENGTH] = "";		// XPath selector for ir events XML node
TCHAR RADEventsXML::m_xirevsevpath[MAXXAPTHLENGTH] = "";	// XPath selector for a single event node
TCHAR RADEventsXML::m_xcolpath[MAXXAPTHLENGTH] = "";		// XPath selector for the single column title node

_bstr_t RADEventsXML::atts[eRADAttMax]; // the 15 attributes as BSTR: all MSXML 3.0 operations use BSTR types
_bstr_t RADEventsXML::bstr_wsn;		// white space for readability
_bstr_t RADEventsXML::bstr_wsnt;
_bstr_t RADEventsXML::bstr_wsntt;

bool RADEventsXML::bInitialized = false; // init toggle flag (see RADEventsXML::InitStatics)

// Output text to stdout and and VS debug window
void dprintf( char * format, ...)
{
	static char buf[1024];
	va_list args;
	va_start( args, format );
	vsprintf( buf, format, args );
	va_end( args);
	OutputDebugStringA( buf);
	printf("%s", buf);
}

///////////////////////////////////////////////////////
// ReportParseError
//
// Utility debui error reporting function. Inoperative in release.
//
// Input:
//	pDom	- document being parsed
//  desc - descriptive string for error reporting
//
///////////////////////////////////////////////////////
void ReportParseError(IXMLDOMDocument *pDom, char *desc) {
	IXMLDOMParseError *pXMLErr=NULL;
	BSTR bstrReason = NULL;
	HRESULT hr;
	HRCALL(pDom->get_parseError(&pXMLErr),
		"dom->get_parseError: ");
	HRCALL(pXMLErr->get_reason(&bstrReason),
		"parseError->get_reason: ");

	dprintf("%s %S\n",desc, bstrReason);
clean:
	if (pXMLErr) pXMLErr->Release();
	if (bstrReason) SysFreeString(bstrReason);
}

PTCHAR RADEventsXML::szDefaultFilename = "RADEvents.xml";

///////////////////////////////////////////////////////
// RADEventsXML::RADEventsXML
//
// Initialize instance variables.
// Initialize static class vars, if first time
//
///////////////////////////////////////////////////////
RADEventsXML::RADEventsXML(void)
{
	InitStatics();

	m_pXMLDom = NULL;
	m_pIRNode =NULL;

	DWORD cchCurDir = MAX_PATH; 
	LPTSTR lpszCurDir; 
	TCHAR tchBuffer[MAX_PATH+1];
	lpszCurDir = tchBuffer; 
	GetCurrentDirectory(cchCurDir, lpszCurDir); 
	lpszCurDir[MAX_PATH] = '\0';

	strcpy(m_path , lpszCurDir);
	strcpy(m_path,".\\");
	strcpy(m_fname, szDefaultFilename);
	sprintf(m_fullpath,"%s%s", m_path, m_fname);
}

///////////////////////////////////////////////////////
// RADEventsXML::~RADEventsXML
//
// El Destructo
//
///////////////////////////////////////////////////////
RADEventsXML::~RADEventsXML(void)
{
	if (m_pXMLDom)
		m_pXMLDom->Release();

	if (m_pIRNode)
		m_pIRNode->Release();
}

///////////////////////////////////////////////////////
// RADEventsXML::InitStatics
//
// Initialize the class static variables only once.
// Toggle on bInitialized
//
///////////////////////////////////////////////////////
void RADEventsXML::InitStatics()
{
	if (bstr_wsn.length() > 0)
		return;

	for (int i = 0; i < eRADAttMax; i++)
		atts[i] = Attributes[i];

	bstr_wsn = L"\n";
	bstr_wsnt= L"\n\t";
	bstr_wsntt=L"\n\t\t";

	strcpy(m_xirevspath,	"//IR/Events");
	strcpy(m_xirevsevpath,	"//IR/Events/Event");
	strcpy(m_xcolpath,		"//IR/Events/Columns");
}

///////////////////////////////////////////////////////
// RADEventsXML::SetLocationPath
//
// Set the partial or full file system path (not including
// the file name), for the persistent external XML file location.
// Default is ".".
//
// Input:
//	path - path name
//
///////////////////////////////////////////////////////
void RADEventsXML::SetLocationPath(PTCHAR pPath)
{
	strcpy(m_path,pPath);
	PTCHAR r = &m_path[strlen(m_path) - 1];
	if (*r && (*r != '\\') != 0)
		strcat(m_path, "\\");
	sprintf(m_fullpath, "%s%s",m_path, m_fname);
}

///////////////////////////////////////////////////////
// RADEventsXML::SetFileName
//
// Set the file name for the persistent external XML file.
// Default is "radfdms.xml"
//
// Input:
//	fname -	file name for the external representation of the list
//
///////////////////////////////////////////////////////
void RADEventsXML::SetFileName(PTCHAR pFName)
{
	strcpy(m_fname, pFName);
	sprintf(m_fullpath, "%s%s",m_path, m_fname);
}

///////////////////////////////////////////////////////
// RADEventsXML::SetFullFilePath
//
// Set the file path and file name for the persistent external XML file.
//
// Input:
//	pPath -	full file path <dir>\\<name> for the external representation of the list
//
///////////////////////////////////////////////////////
void RADEventsXML::SetFullFilePath(PTCHAR pPath)
{
	TCHAR _path[MAXRADFDMSPATHLEN];
	strcpy(_path, pPath);
	char *p;
	char* filename;
	p = strrchr(_path , '\\' );
	if (p)
	{
		int result;
		result = p - _path + 1;
		
		filename =  _path  + result;
		strcpy(m_fname, filename);   // grab the name

		*(p+1) = '\0';
		strcpy(m_path,_path);  // trim the name and grab the path

	}
	else				// not sure what this is, jst a file name probably
	{
		filename = pPath;
		strcpy(m_fname, filename);
		strcpy(m_path,".\\");
	}
	sprintf(m_fullpath, "%s%s",m_path, m_fname);


}

///////////////////////////////////////////////////////
// RADEventsXML::XMLToEvent
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
RADEvent* RADEventsXML::XMLToEvent(IXMLDOMNode* pEvent)
{
	HRESULT hr;
	RADEvent* p = NULL;

	IXMLDOMNode *pAtt=NULL;
	IXMLDOMNamedNodeMap* pat = NULL;
	long l;
	int al;

	HRCALL(pEvent->get_attributes(&pat), "get_attributes: ");
	HRCALL(pat->get_length(&l), "get_length: ");
	ASSERT(l <= (int)eRADAttMax);

	p = new RADEvent();
	for (al = 0; al < l; al++)
	{
		HRCALL(pat->getNamedItem(atts[al], &pAtt), "gni");
		if (pAtt)
		{
			VARIANT varValue;
			::VariantInit(&varValue);
			pAtt->get_nodeValue(&varValue);
			p->SetREValue((RADEventField)al, varValue.bstrVal);
			::VariantClear(&varValue);
			pAtt->Release();
		}
	}

clean: 
	if (pat)
		pat->Release();
	return p;
}


///////////////////////////////////////////////////////
// RADEventsXML::ProcessEvents
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
bool RADEventsXML::ProcessEvents(RADEventsList& rel, bool bExtract)
{

	// if bExtract, we want to read the XML events and populate the list of event objects
	// first: get count of events nodes
	// second:
	//	for each event node
	//    get the node
	//    create a RADEvent, using the node attributes to populate it
	//	  insert the RADEvent object into the RADEventsList 'rel'
	// end
	// else, not bExtract, we want to transform the list of RADEvent objects into an XML document
	// for each RADEvent on the RADEventsList 'rel,
	//    create a new XML Event Node, using each stirng value as an attribute value
	//    append the XML Event node to the <IR/Events> node
	//  end

	HRESULT hr;
	_bstr_t xbstr = m_xirevsevpath;

	if (bExtract)  // converting from XML <Event> to RADEvent
	{
		IXMLDOMNodeList* pXMLDomNodeList = NULL;
		HRCALL(m_pXMLDom->selectNodes(xbstr, &pXMLDomNodeList), "selectNodes");
		if (pXMLDomNodeList)
		{
			long count = 0;
			HRCALL(pXMLDomNodeList->get_length(&count), "get_length");
			IXMLDOMNode *pEventNode=NULL;
			for (long i=0; i<count; i++)
			{
				HRCALL(pXMLDomNodeList->get_item(i, &pEventNode), "get_item: ");
				if (bDebug)
				{
					BSTR dbg1str;
					BSTR dbg2str;
					HRCALL(pEventNode->get_nodeName(&dbg1str), "get_nodeName: ");
					dprintf("Node (%d), <%S>:\n",i, static_cast<wchar_t*>(dbg1str));
					HRCALL(pEventNode->get_xml(&dbg2str), "get_xml: ");
					dprintf("\t%S\n",  static_cast<wchar_t*>(dbg2str));
				}

				RADEvent* p = XMLToEvent(pEventNode);
				rel.AddToEnd(p);
				if (bDebug)
				{
					dprintf(p->Image());
				}
				if (pEventNode) pEventNode->Release();
			}
			pXMLDomNodeList->Release();
		}
		else
		{
			ReportParseError(m_pXMLDom, "Error while calling ProcessEvents/selectNodes ");
		}
	}
	else // converting from RADEvent to XML <Event>
	{
		IXMLDOMElement * pE;
		IXMLDOMNode *pIXMLDOMNode = NULL;
		long i = 0;
		POSITION pos = rel.GetHeadPosition();
		while( pos != NULL )
		{
			RADEvent* p = (RADEvent*)rel.GetNext( pos );
			pE = ConstructEventNode(p);
			if (pE)
			{
				HRCALL(m_pIRNode->appendChild(pE,&pIXMLDOMNode), ""); 
				AddWhiteSpaceToNode(bstr_wsnt, m_pIRNode);
				i++;
			}
		}
	}

clean: 
	if (FAILED(hr))
		return false;
	else
		return true;
}

///////////////////////////////////////////////////////
// RADEventsXML::ConstructXMLDoc
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
bool RADEventsXML::ConstructXMLDoc(RADEventsList& rel)
{
	_bstr_t bstr;
	HRESULT hr;
	IXMLDOMNode *pColNode = NULL;
	IXMLDOMNode *pIXMLDOMNode = NULL;

	if (this->m_pXMLDom != NULL) 
	{
		if (this->m_pIRNode == NULL)
			//  might need to create the entire tree, the file didn't exist
			// or  might need to just create the tree down to the Events level
		{
			this->m_pIRNode = ConstructEventParentNodeTree();
		}
		if (this->m_pIRNode == NULL )
			return false;  // attempt to create IR/Events has failed

		bstr = m_xcolpath;
		HRCALL(m_pXMLDom->selectSingleNode(bstr, &pColNode), "selectSingleNode");

		if (pColNode == NULL)
		{
			pColNode = ConstructColumnsNode();
			if (pColNode)
			{
				HRCALL(m_pIRNode->appendChild(pColNode,&pIXMLDOMNode), ""); 
				AddWhiteSpaceToNode(bstr_wsnt, m_pIRNode);
			} else // if (pColNode == NULL)
				return false; // forced re-selection of the newly constructed node tree has failed
		}
	}
	else
		return false;  // no DOM means punt
clean:
	return ProcessEvents(rel, false);  // build the event list on the IR/Events tree
}


///////////////////////////////////////////////////////
// RADEventsXML::ConstructEventParentNodeTree
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
IXMLDOMElement* RADEventsXML::ConstructEventParentNodeTree()
{
	IXMLDOMNode *pIXMLDOMNode = NULL;
	IXMLDOMElement *pIR = NULL;
	IXMLDOMElement *pEvents = NULL;
	IXMLDOMProcessingInstruction *pi=NULL;

	_bstr_t bstr1;
	_bstr_t bstr2;
	HRESULT hr;

	// Create a processing instruction element. <?xml version="1.0" ?>
	bstr1 = "xml";
	bstr2 = "version='1.0'";
	HRCALL(m_pXMLDom->createProcessingInstruction(
		bstr1,bstr2, &pi),
		"createProcessingInstruction:");
	m_pXMLDom->appendChild(pi, &pIXMLDOMNode);  // put it first

	// construct <IR><Events></Events></IR>
	bstr1 = "Events";
	HRCALL(m_pXMLDom->createElement(bstr1,&pEvents),"create <Events> ");

	bstr1 = "IR";
	HRCALL(m_pXMLDom->createElement(bstr1,&pIR),"create <IR>");

	HRCALL(pIR->appendChild(pEvents, &pIXMLDOMNode), ""); 
	AddWhiteSpaceToNode(bstr_wsnt, pIR);

	m_pXMLDom->appendChild(pIR, &pIXMLDOMNode);   // attach to DOM

clean: 
	if (pi)
		pi->Release();
	if (pIR)
		pIR->Release();
	return pEvents;
}

///////////////////////////////////////////////////////
// RADEventsXML::ConstructEventNode
//
// Creates an XMLEvent node from a RADEvent object
//
// An example XML node as text:
//	<Event num="0057"
//	starttime="0000020010826175602000" endtime="0000020010826175827000"
//	maxtime="0000020010826175702000"
//	sum="10941.0" maxvalue="84"
//	starttimedisplay="2001.08.26 - 17:56:02"
//	endtimedisplay="2001.08.26 - 17:58:27"
//	maxtimedisplay="2001.08.26 - 17:57:02"
//	etype="Channel"
//	stationname="GRAND 2" />
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
IXMLDOMElement* RADEventsXML::ConstructEventNode(RADEvent* pEvent)
{
	IXMLDOMElement *pEventElement = NULL;
	_variant_t var1;
	_bstr_t bstr1;
	HRESULT hr;

	IXMLDOMAttribute *pAttNode=NULL;
	IXMLDOMNode *pIXMLDOMNode=NULL;
	IXMLDOMNamedNodeMap* pAttMap = NULL;
	int j;

	// construct <Event>
	bstr1 = "Event";
	HRCALL(m_pXMLDom->createElement(bstr1,&pEventElement),"create <Event> ");
	HRCALL(pEventElement->get_attributes(&pAttMap), "get_attributes: ");

	for (j = 0; j < eRADAttMax; j++)
	{
		var1 = pEvent->values[j];
		HRCALL(m_pXMLDom->createAttribute(atts[j], &pAttNode), "create attribute");
		HRCALL(pAttNode->put_nodeValue(var1), "node value");
		HRCALL(pAttMap->setNamedItem(pAttNode, &pIXMLDOMNode), "set named item");
		if (pIXMLDOMNode)
			pIXMLDOMNode->Release();
	}

clean: 
	if (pAttMap)
		pAttMap->Release();
	if (pAttNode)
		pAttNode->Release();

	return pEventElement;
}

// try this alternate error handling macro
inline void TESTHR( HRESULT _hr ) { if FAILED(_hr) throw(_hr); }

///////////////////////////////////////////////////////
// RADEventsXML::ConstructColumnsNode
//
// Creates an XML Event node for the Columns node.
// The Columns node is a singular node placed under the <IR/Events>
// node.
/* <Columns>
		<Title>RAD-Sequence Num</Title>
		<Title>RAD-Start Timestamp</Title>
		<Title>RAD-End Timestamp</Title>
		<Title>RAD-Max Timestamp</Title>
		<Title>RAD-Integral Sum</Title>
		<Title>RAD-Max Value</Title>
		<Title>RAD-Max Reals Rate</Title>
		<Title>RAD-Max Totals Rate</Title>
		<Title>Station Name</Title>
		<Title>Station Type</Title>
		<Title>RAD-Event Channel</Title>
		<Title>RAD-Type</Title>
		<Title>RAD-SubType</Title>
		<Title>RAD-Data Point Num</Title>
		<Title>RAD-Event ID</Title>
 </Columns> */
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
IXMLDOMElement* RADEventsXML::ConstructColumnsNode()
{
	IXMLDOMElement *pColumns = NULL;
	IXMLDOMElement *pTitle = NULL;
	IXMLDOMNode *pIXMLDOMNode = NULL;

	_bstr_t bstr1;
	_bstr_t bstr2;
	HRESULT hr = 0;

	try
	{
		// create <Columns>
		bstr1 = "Columns";
		hr = m_pXMLDom->createElement(bstr1,&pColumns);
		TESTHR(hr);
		AddWhiteSpaceToNode(bstr_wsnt, pColumns);

		// for each title element
		//	create <Title>value</Title>
		bstr2 = "Title";
		for (int i = 0; i < eRADAttMax; i++)
		{
			pTitle = NULL;
			//	create <Title>value</Title> 
			hr = m_pXMLDom->createElement(bstr2,&pTitle);
			TESTHR(hr);

			bstr1 = RADEventsList::Titles[i];
			hr = pTitle->put_text(bstr1); 
			TESTHR(hr);

			//  append it as a child to <Columns>
			hr = pColumns->appendChild(pTitle, &pIXMLDOMNode); 
			TESTHR(hr);
			AddWhiteSpaceToNode(bstr_wsnt, pColumns);
		}
	}
	catch(...)
	{
		dprintf( "%s:%d  HRCALL Failed: %s\n  0x%.8x = %s\n",
			__FILE__, __LINE__, "ConstructColumnsNode Exception" , hr);
	}
	return pColumns;
}

///////////////////////////////////////////////////////
// RADEventsXML::Persist
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
void RADEventsXML::Persist(RADEventsList& rel)
{
	HRESULT hr = 0;

	DomFromCOM();
	if (!m_pXMLDom) throw NULL;

	try
	{
		if (rel.bChanged)
			ConstructXMLDoc(rel);

		if (m_pXMLDom)
		{
			_variant_t vSrc = m_fullpath;
			hr = m_pXMLDom->save(vSrc);
			TESTHR(hr);
			rel.bChanged = false;
		}
	}
	catch(...)
	{
		dprintf( "%s:%d  HRCALL Failed: %s\n  0x%.8x = %s\n",
			__FILE__, __LINE__, "Persist Exception" , hr);
	}
}

///////////////////////////////////////////////////////
// RADEventsXML::Populate
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
void RADEventsXML::Populate(RADEventsList& rel)
{
	/*  using the path and file name,
	find the XML file,
	open it in memory,
	get the event tree from it,
	extract the n events on the event tree,
	populate the local data structure with thr n events
	*/

	HRESULT hr;
	IXMLDOMParseError * pObjError = NULL;
	VARIANT_BOOL status;
	_variant_t vSrc;
	_bstr_t bstr;
	_bstr_t filepath(m_fullpath);
	
	DomFromCOM();
	if (!m_pXMLDom) goto clean;

	vSrc = m_fullpath;

	hr = m_pXMLDom->load(vSrc, &status);

	if(status!=VARIANT_TRUE)
	{
		hr = m_pXMLDom->get_parseError(&pObjError);
		VARIANT varValue;
		::VariantInit(&varValue);
		hr = pObjError->get_reason( &(varValue.bstrVal));
		dprintf("Failed to load DOM from %s. %S\n",m_fullpath, varValue.bstrVal);
		::VariantClear(&varValue);
		goto clean;
	}

	if (m_pIRNode)
		m_pIRNode->Release();
	m_pIRNode = NULL;
	bstr = m_xirevspath;
	HRCALL(m_pXMLDom->selectSingleNode(bstr, &m_pIRNode), "selectSingleNode");
	if (m_pIRNode)
	{
		ProcessEvents(rel, true);
	}

clean: 
	if (pObjError)
		pObjError->Release();
}



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
void RADEventsXML::DomFromCOM()
{
	HRESULT hr;

	if (m_pXMLDom)
		return;

	// explicitly use the MSXML 3.0 API! Fails if it is not installed
	HRCALL( CoCreateInstance(__uuidof(DOMDocument30),
		NULL,
		CLSCTX_INPROC_SERVER,
		__uuidof(IXMLDOMDocument),
		(void**)&m_pXMLDom),
		"Create a new DOMDocument");

	// no validation ... ignore externals ... synchronous evaluation
	HRCALL( m_pXMLDom->put_async(VARIANT_FALSE),       "should never fail");
	HRCALL( m_pXMLDom->put_validateOnParse(VARIANT_FALSE), "should never fail");
	HRCALL( m_pXMLDom->put_resolveExternals(VARIANT_FALSE),  "should never fail");

clean:
	;

}


///////////////////////////////////////////////////////
// RADEventsXML::AddWhiteSpaceToNode
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
void RADEventsXML::AddWhiteSpaceToNode(
									   BSTR bstrWs,
									   IXMLDOMNode *pNode)
{
	HRESULT hr;
	IXMLDOMText *pws=NULL;
	IXMLDOMNode *pBuf=NULL;
	HRCALL(this->m_pXMLDom->createTextNode(bstrWs,&pws), " ");
	HRCALL(pNode->appendChild(pws,&pBuf)," ");
clean:
	if (pws) pws->Release();
	pws=NULL;
	if (pBuf) pBuf->Release();
	pBuf=NULL;
}


