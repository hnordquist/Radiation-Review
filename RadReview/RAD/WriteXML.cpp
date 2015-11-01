// $History: test.cpp $
// 
// *****************  Version 2  *****************
// User: Longo        Date: 12/09/04   Time: 12:55p
// Updated in $/FDMS/FDMS Application/FDMSNET/tests
// 
// *****************  Version 1  *****************
// User: Longo        Date: 10/20/04   Time: 1:11p
// Created in $/FDMS/FDMS Application/FDMSNET/tests
// registry failed today, baselining

#include "RadBuild.h"
//#include "stdafx.h"

#include "EventTypeLists.h"
#include "FacilityConfig.h"
#include "MyDateTime.h"
#include "RadEventList.h"
#include "WriteXML.h"

extern short glsFacNum;
extern CMyDateTime glMyDateTime;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define STR_LEN 256

static int iSequenceNumber = 0;
static RADEventsList* pRadEventsList = NULL;		// all the events added to list and then finally written to XML

// 03-Jan-2005	Modified to write 15 fields rather than 11
void WriteEventSummaryToXmlFile(struct db_event_rec *pdbEvent, short sAction, char* pszFileNameAndPath)
{   
	char szFile[STR_LEN+1];
	// create a new RAD Event List, if necessary
	if ((pRadEventsList == NULL) || (sAction == FIRST_EVENT)) {
		pRadEventsList = new RADEventsList();
		iSequenceNumber = 0;
	}

	if (sAction == WRITE_EVENT_LIST_TO_XML) {
		// Save the 2 element event list to an external location.
		// Persist is the operation that creates (or overwrites the pre-existing) ".\jeeves.xml".
		pRadEventsList->Persist();

		// Delete the RADEvents list object, thelist destructor also deletes each RADEvent object added to the list above
		delete pRadEventsList;
		pRadEventsList = NULL;
		iSequenceNumber = 0;
		return;
	}

	// The external location and name of the RAD to FDMS data interchange file are predefined as .\radfdms.xml,
	// The file name and path can be manually set with these two operations
	pglFacCfg->GetFacilityDirectory(glsFacNum, szFile, STR_LEN);
	strncat(szFile, "\\data\\", STR_LEN);
	pRadEventsList->SetLocationPath(szFile);	// create the external file in the current working directory
	//pRadEventsList->SetLocationPath(".");		// create the external file in the current working directory
	pRadEventsList->SetFileName("RadEvents.xml"); // set the name of the external file
	strncat(szFile, "RadFdms.xml", STR_LEN);	// Create name and path to display in RAD results
	// create a new event object (this will be deleted when the whole list is deleted)
	RADEvent* pRADEvent = new RADEvent();
	strcpy(pszFileNameAndPath, szFile);

	// place string representations of each of the 15 RAD values onto the RADEvent object
	char szStr[STR_LEN+1];	// create temporary strings here
	char szStartDateTime[STEVES_TIMESTAMP_LEN+1], szEndDateTime[STEVES_TIMESTAMP_LEN+1], szMaxDateTime[STEVES_TIMESTAMP_LEN+1];

	// 1. Fill eRADSequenceNum
	sprintf(szStr, "%04d", iSequenceNumber);
	pRADEvent->SetREValue(eRADSequenceNum, szStr);

	// 2. Fill eRADStartTimestamp
	//Date.DATETimestampToStevesTimestampStrings(pdbEvent->d_event_beg_time_key, szStr, szStartDateTime);
	glMyDateTime.DATETimestampToStevesTimestampStrings(pdbEvent->d_event_beg_time_key, szStr, szStartDateTime);
	pRADEvent->SetREValue(eRADStartTimestamp,szStr);

	//	3 .Fill eRADEndTimestamp
	//Date.DATETimestampToStevesTimestampStrings(pdbEvent->d_event_end_time, szStr, szEndDateTime);
	glMyDateTime.DATETimestampToStevesTimestampStrings(pdbEvent->d_event_end_time, szStr, szEndDateTime);
	pRADEvent->SetREValue(eRADEndTimestamp, szStr);
	
	//	4. Fill eRADMaxTimestamp
	//Date.DATETimestampToStevesTimestampStrings(pdbEvent->d_event_max_time, szStr, szMaxDateTime);
	glMyDateTime.DATETimestampToStevesTimestampStrings(pdbEvent->d_event_max_time, szStr, szMaxDateTime);
	pRADEvent->SetREValue(eRADMaxTimestamp, szStr);
	
	//	5. Fill eRADIntegralSum
	// Print the integral sum, based on value of number use one of three formats
	if (pdbEvent->d_event_integral_sum < 999.0)  { // small number
		if (pdbEvent->d_event_integral_sum > 10.0)  {
			sprintf(szStr, "%12.1f ", pdbEvent->d_event_integral_sum);	// Integral Sum
		}
		else {	// smallest
			sprintf(szStr,"%12.4f ", pdbEvent->d_event_integral_sum);	// Integral Sum
		}
	} 
	else {	// large number
		sprintf(szStr,"%14.1f ", pdbEvent->d_event_integral_sum);		// Integral Sum
	}	
	pRADEvent->SetREValue(eRADIntegralSum, szStr);
	
	//	6. Fill eRADMaxValue
	// Print the maximum value, based on value of number use one of two formats
	if (pdbEvent->d_event_max_value > 10) {	// large positive value
		sprintf(szStr,"%9.1f ", pdbEvent->d_event_max_value);	// maximum value   
	}
	else {	// small positive value	
		if (pdbEvent->d_event_max_value > 0) {	
			sprintf(szStr,"%9.5f ", pdbEvent->d_event_max_value);	// maximum value   	
		}
		else { // negative value
			sprintf(szStr,"%9.1f ", pdbEvent->d_event_max_value);	// maximum value   	
		}
	}
	pRADEvent->SetREValue(eRADMaxValue,szStr);

	//	7. Fill eRADAvgRealsRate
	sprintf(szStr,"%12.2lf ", pdbEvent->d_event_avg_reals_rate);
	pRADEvent->SetREValue(eRADAvgRealsRate,szStr);

	//	8. Fill eRADAvgTotalsRate
	sprintf(szStr,"%12.2lf ", pdbEvent->d_event_avg_totals_rate);
	pRADEvent->SetREValue(eRADAvgTotalsRate,szStr);

	//	9. Fill eRADStationName
	IDStruct TempID;
	TempID.sStaID = pdbEvent->us_event_sta;
	TempID.sStaType = pdbEvent->us_event_sta_type;
	pglFacCfg->GetStationLongName(glsFacNum, &TempID, szStr, STR_LEN);
	pRADEvent->SetREValue(eRADStationName, szStr);

	//	10. Fill eRADStationType
	pRADEvent->SetREValue(eRADStationType, TempID.szStaType);
	
	//	11. Fill eRADEventChannel (long)
	sprintf(szStr,"%3d ", pdbEvent->us_event_chan);
	pRADEvent->SetREValue(eRADEventChannel, szStr);

	//	12. Fill eRAD EventType (Unknown, Channel, Measurement or Background)
	GetEventType1Str(pdbEvent, szStr);
	pRADEvent->SetREValue(eRADEventType, szStr);

	//	13. Fill eRADEventSubType "Subtype" of event 
	//	(For Measurement is Unknown, Empty, Assay, Cf Check, Cf Normalization or Totals)  
	//	(For Background is Normal Background, Low Background or High Background)
	GetEventType2Str(pdbEvent, szStr);
	pRADEvent->SetREValue(eRADEventSubType, szStr);

	//	14. Fill eRADDataPointNum
	sprintf(szStr,"%5d ", pdbEvent->s_num_runs);
	pRADEvent->SetREValue(eRADDataPointNum, szStr);

	//	15. Fill eRADEventID, Event ID (up to 12 characters)
	pRADEvent->SetREValue(eRADEventID, pdbEvent->sz_event_id);
	
	// add the RAD Event to the Event List
	pRadEventsList->AddToEnd(pRADEvent);

	iSequenceNumber++;

	return; 
}

