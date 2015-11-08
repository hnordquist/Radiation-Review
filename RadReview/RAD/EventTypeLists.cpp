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
///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : EventTypeLists.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////

#include "RAD.H"    
#include "EventTypeLists.h"
#include "FacilityConfig.h"		//KM
#include "TypeDefinitions.h"
                            
#define MAX_TYPES 20                            

static short gllsTypeNums[MAX_TYPES];
extern short glsFacNum; //KM
//note:  the choices are defines in the raddefs.h file.  If you change any item in the picklists below, be sure to 
// modify the accompanying definitions for the picklist choices in the raddefs.h file.  
// This is messy and should be optimized at a later time.
// 16-Dec-2004	SFK Corrected list of graph event type choices for Reals channel
#define GRAPH_REALS_LIST_CHOICES 8
static char *gllnaszGraphRealsList[] = {"Verification       ",
								        "Cf Normalization   ",
								        "Background         ",
								        "Cf Check           ",
								        "Empty Verification ",
										"Low Background     ",
								        "High Background    ",
										"Channel Event      ",
								         NULL};

#define GRAPH_TOTALS_LIST_CHOICES 3
static char *gllnaszGraphTotalsList[] = {"Totals Verification",
										 "Background         ",
										 "Channel Event      ",
								          NULL};

#define SUMMARY_LIST_CHOICES 9
static char *gllnaszSummaryList[] = {"Verification       ",
								     "Cf Normalization   ",
								     "Background         ",
								     "Totals Verification",
								     "Cf Check           ",
								     "Empty Verification ",
								     "Low Background     ",
								     "High Background    ",
								     "All                ",
								      NULL};
#define EXPORT_LIST_CHOICES 5
static char *gllnaszExportList[] = {"Verification         ",
								    "Cf Normalization     ",
								    "Background           ",
								    "Empty Verification   ",
								    "Totals Verification  ",
								     NULL};

#define GRAPH_LIST_CHOICES 1
static char *gllnaszGraphList[] = {"Channel Event    ",
								    NULL};

// 10-Dec-2004 SFK Added another list to be associated with GRAND data
#define GRAPH_GRAND_LIST_CHOICES 2
static char *gllnaszGraphGrandList[] = {"Channel Event    ",
								        "Background       ",		
								         NULL};

       
/*===========================================================================
 *
 *  Name	 : 	GetEventTypePicklist 
 *
 *  Purpose	 : 	Create a list of addresses associated with event types. Also create
 *				an associated array of unique numbers that will be used to
 *				quickly find the associated types
 *
 *  Return value :  TRUE/FALSE
 *
 *  Special notes:  Caller must deallocate the memory for pick list addresses
 *					when through, using gen_deallocate_str_array().
 *				    If there is insufficient memory available, the return
 *				    will be FALSE and the pick list address is meaningless.
 *
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  20-May-1998 SFK		Created based on facility, station and channel picklist routines
 *	02-Oct-2003	SFK		Modified for new FacMgr
 *	10-Dec-2004	SFK		Modified to add a new list specific to GRAND data
 *	13-Dec-2004	SFK		Modified to give different choices to the export list if GRAND
 *
===========================================================================*/
int GetEventTypePicklist(struct IDStruct ChanID, short sListType, char **pList[], short *psNumChcs)
{
    
	struct db_chan_rec dbChanRec;      
	int iStationType; //KM
   /* ------------------------------------------------------------------
    *	Figure out what kind of channel this is as SR Reals data are
	*	a special case. (-1 signals JSR type data)
    * ----------------------------------------------------------------*/
	if (ChanID.sChanID != -1) {
		if (!pglFacCfg->GetChannelRecord(glsFacNum, &ChanID, &dbChanRec)) return(FALSE);
		iStationType = ChanID.sStaType;
	}
	else {
		iStationType = JSR_TYPE; //-1 indicates know have a JSR type   //km
	}

	switch (sListType) {
	case EXPORT_LIST:
		if ((iStationType == JSR_TYPE) || (iStationType == JSR1_TYPE) || (iStationType == ISR_TYPE)) {
			*pList = gllnaszExportList;
			*psNumChcs = EXPORT_LIST_CHOICES;
		}
		if (iStationType == GRAND_TYPE) {
			*pList = gllnaszGraphGrandList;
			*psNumChcs = GRAPH_GRAND_LIST_CHOICES;
		}
		break;

	case SUMMARY_LIST:
		*pList = gllnaszSummaryList;
		*psNumChcs = SUMMARY_LIST_CHOICES + 1;
		break;
	
	case GRAPH_LIST:
		// choices for normal channels
		*pList = gllnaszGraphList;
		*psNumChcs = GRAPH_LIST_CHOICES;
		// special choices for reals and totals channels for SR instruments
		if ((iStationType == JSR_TYPE)  || (iStationType == JSR1_TYPE) || (iStationType == ISR_TYPE)) {		//km
			if (dbChanRec.s_chan_offset == (REALS_OFFSET+1)) {
				*pList = gllnaszGraphRealsList;
				*psNumChcs = GRAPH_REALS_LIST_CHOICES;
			}
			if (dbChanRec.s_chan_offset == (TOTALS_OFFSET+1)) {
				*pList = gllnaszGraphTotalsList;
				*psNumChcs = GRAPH_TOTALS_LIST_CHOICES;
			}
		}
		// 12-Dec-2004	Add GRAND list
		if (iStationType == GRAND_TYPE) {
			*pList = gllnaszGraphGrandList;
			*psNumChcs = GRAPH_GRAND_LIST_CHOICES;
		}

		break;

	default:
			*pList = NULL;
			*psNumChcs = 0;
	}

    return(TRUE);
}


/* <f> */       
/*===========================================================================
 *
 *  Name	 :  GetEventTypesForPlNum 
 *				GetPlNumForEventTypes
 *
 *  Purpose	 : 	Given the channel number and the event types, find the 
 *				associated picklist position number in gllnaszLongList.
 *				Given the channel number and picklist position number in
 *				gllnaszLongList, find the event types.
 *				These routines were created for use with the graphics where
 *				events can be created or modified and isolates the graphics
 *				routines from having to know about event types and channels.
 *
 *  Return value :  TRUE/FALSE
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:

 *  date    /	author	revision
 *  -----------------	--------
 *  20-May-1998 SFK		Created
 *	02-Oct-2003	SFK		Modified for new FacMgr
 *	10-Dec-2004	SFK		Modify to return events types for GRAND data of channel or background
 *	16-Dec-2004	SFK		Added background to totals choices
===========================================================================*/
int GetEventTypesForPlNum(struct IDStruct ChanID, unsigned int uiChc, unsigned short *pusType1, unsigned short *pusType2)
{
	struct db_chan_rec dbChanRec;      

	if (!pglFacCfg->GetChannelRecord(glsFacNum, &ChanID, &dbChanRec)) return(FALSE);

	// types for normal channels which have no choices
	*pusType1 = CHANNEL_EVENT;
	*pusType2 = UNKNOWN_MEAS;

	// types for special cases of reals and totals channels for SR instruments
	if ((ChanID.sStaType == JSR_TYPE)  || (ChanID.sStaType == JSR1_TYPE) || (ChanID.sStaType == ISR_TYPE)) {		//km
		if (dbChanRec.s_chan_offset == (REALS_OFFSET+1)) {
			// is a JSR and this is the reals channel, a special case
			switch (uiChc) {
				
			case REALS_ASSAY_PL_CHC:
				*pusType1 = MEASUREMENT_EVENT;
				*pusType2 = ASSAY_MEAS;
				break;

			case REALS_CF_NORM_PL_CHC:
				*pusType1 = MEASUREMENT_EVENT;
				*pusType2 = CF_NORM_MEAS;
				break;

			case REALS_NORM_BKG_PL_CHC:
				*pusType1 = BACKGROUND_EVENT;
				*pusType2 = NORMAL_BKG_MEAS;
				break;

			case REALS_CF_CHECK_PL_CHC:
				*pusType1 = MEASUREMENT_EVENT;
				*pusType2 = CF_CHECK_MEAS;
				break;

			case REALS_EMPTY_ASSAY_PL_CHC:
				*pusType1 = MEASUREMENT_EVENT;
				*pusType2 = EMPTY_MEAS;
				break;

			case REALS_LOW_BKG_PL_CHC:
				*pusType1 = BACKGROUND_EVENT;
				*pusType2 = LOW_BKG_MEAS;
				break;

			case REALS_HIGH_BKG_PL_CHC:
				*pusType1 = BACKGROUND_EVENT;
				*pusType2 = HIGH_BKG_MEAS;
				break;

			case REALS_CHAN_PL_CHC:
				*pusType1 = CHANNEL_EVENT;
				*pusType2 = UNKNOWN_MEAS;
				break;
			
			default:
				*pusType1 = UNKNOWN_EVENT;
				*pusType2 = UNKNOWN_MEAS;
				break;
			}
		}
		if (dbChanRec.s_chan_offset == (TOTALS_OFFSET+1)) {
			switch (uiChc) {
			case TOTALS_CHAN_PL_CHC:
				*pusType1 = CHANNEL_EVENT;
				*pusType2 = UNKNOWN_MEAS;
				break;
			case TOTALS_ASSAY_PL_CHC:
				*pusType1 = MEASUREMENT_EVENT;
				*pusType2 = TOTALS_MEAS;
				break;
			case TOTALS_BKG_PL_CHC:
				*pusType1 = BACKGROUND_EVENT;
				*pusType2 = NORMAL_BKG_MEAS;
				break;
			default:
				*pusType1 = UNKNOWN_EVENT;
				*pusType2 = UNKNOWN_MEAS;
				break;
			}
		}
	}

	//12-Dec-2004	Added choices for GRAND lists
	if (ChanID.sStaType == GRAND_TYPE) {
		switch (uiChc) {
			case GRAND_CHAN_PL_CHC:
				*pusType1 = CHANNEL_EVENT;
				*pusType2 = UNKNOWN_MEAS;
				break;
			case GRAND_BKGD_PL_CHC:
				*pusType1 = BACKGROUND_EVENT;
				*pusType2 = NORMAL_BKG_MEAS;
				break;
			default:
				*pusType1 = UNKNOWN_EVENT;
				*pusType2 = UNKNOWN_MEAS;
				break;
			}
	}
    return(TRUE);
}


int GetPlNumForEventTypes(struct IDStruct ChanID, unsigned int *puiChc, unsigned short usType1, unsigned short usType2)
{
	struct db_chan_rec dbChanRec;      

	if (!pglFacCfg->GetChannelRecord(glsFacNum, &ChanID, &dbChanRec)) return(FALSE);;

	// picklist choice for channels with default choices
	*puiChc = GRAPH_CHAN_EVENT_PL_CHC;  //channel event
	// picklist choice for special cases of reals and totals channels for SR instruments
	if ((ChanID.sStaType == JSR_TYPE)  || (ChanID.sStaType == JSR1_TYPE) || (ChanID.sStaType == ISR_TYPE)) 
	{			//km
		if (dbChanRec.s_chan_offset == (REALS_OFFSET+1)) 
		{
			// know this is a reals channel so get pl choice from gllszGraphRealsList
			if (usType1 == MEASUREMENT_EVENT) 
			{
				switch (usType2) 
				{
				case ASSAY_MEAS:
					*puiChc = REALS_ASSAY_PL_CHC;
					break;

				case CF_NORM_MEAS:
					*puiChc = REALS_CF_NORM_PL_CHC;
					break;

				case CF_CHECK_MEAS:
					*puiChc = REALS_CF_CHECK_PL_CHC;
					break;

				case EMPTY_MEAS:
					*puiChc = REALS_EMPTY_ASSAY_PL_CHC;
					break;
				}
			}
		
			if (usType1 == BACKGROUND_EVENT) 
			{
				switch (usType2) 
				{
				case NORMAL_BKG_MEAS:
					*puiChc = REALS_NORM_BKG_PL_CHC;
					break;

				case HIGH_BKG_MEAS:
					*puiChc = REALS_HIGH_BKG_PL_CHC;
					break;

				case LOW_BKG_MEAS:
					*puiChc = REALS_LOW_BKG_PL_CHC;
					break;
				}
			}
			if (usType1 == CHANNEL_EVENT) 
			{
				*puiChc = REALS_CHAN_PL_CHC;
			}
		}
		if (dbChanRec.s_chan_offset == (TOTALS_OFFSET+1)) 
		{
			// know this is a totals channel so get pl choice from gllszGraphTotalsList
			if ((usType1 == MEASUREMENT_EVENT) && (usType2 ==  TOTALS_MEAS)) 
			{
				*puiChc = TOTALS_ASSAY_PL_CHC;
			}
			else if (usType1 == BACKGROUND_EVENT)  //SCR 324 pjm 04-01-08
			//if (usType1 == BACKGROUND_EVENT)
			{
				*puiChc = TOTALS_BKG_PL_CHC;
			}
			else 
			{
				*puiChc = TOTALS_CHAN_PL_CHC;
			}
		}
	}
	//12-Dec-2004	Added choices for GRAND lists
	if (ChanID.sStaType == GRAND_TYPE) 
	{
		if (usType1 == CHANNEL_EVENT) 
			*puiChc = GRAND_CHAN_PL_CHC;

		if (usType1 == BACKGROUND_EVENT) 
			*puiChc = GRAND_BKGD_PL_CHC;
	}
	return(TRUE);
}
	                                                     

///////////////////////////////////////////////////////////////////////////
//	Name:	GetEventTypeStr
//
//	Description:
//  Return a string based on the Channel event type parameter 
//
//	Declaration:
//	static void GetEventTypeStr(struct db_event_rec *pdbEvent, char *pszEventType)
//
//	Input:	pdbEvent	event of interest
//			
//	Output:	none
//
//	Return:	string with type identifier
//	
//  date    /	author	revision
//  -----------------	--------
//	???			SFK		Created
//	24-Jun-2002	SFK		Changed name from ConvertChanEventTypeToStr
//	10-Dec-2004	SFK		Added Background Event Type
//	14-Dec-2004	SFK		Moved function from FindEvent and generalized to print
//						all event strings needed by RAD
//	03-Jan-2005	SFK		Added routines to return strings for the two different types
//////////////////////////////////////////////////////////////////
void GetEventTypeStr(struct db_event_rec *pdbEvent, char *pszEventType)
{

	switch (pdbEvent->us_event_type) {	// event type
		case NOT_RECONCILED_EVENT:
			strcpy(pszEventType, "Unreconciled");
			break;
																					
		case RECONCILED_EVENT:						
			strcpy(pszEventType, "Reconciled  ");
			break;
																					
		case UNEXPLAINED_EVENT:
			strcpy(pszEventType, "Unexplained ");
			break;
																					
		case MISSING_DATA_EVENT:
			strcpy(pszEventType, "Missing Data");
			break;
			
		case CHANNEL_EVENT:
			strcpy(pszEventType, "Channel     ");															
			break;
			
		case BACKGROUND_EVENT:
			switch (pdbEvent->us_event_type1) {
			case LOW_BKG_MEAS:															
				strcpy(pszEventType, "Low Bkg     ");
				break;
																						
			case HIGH_BKG_MEAS:															
				strcpy(pszEventType, "High Bkg    ");
				break;

			default :
				strcpy(pszEventType, "Background  ");															
				break;
			}
			break;

		case MEASUREMENT_EVENT:
			switch (pdbEvent->us_event_type1) {							// measurement type
			case EMPTY_MEAS:															
				strcpy(pszEventType, "Empty       ");
				break;
																						
			case ASSAY_MEAS:															
				strcpy(pszEventType, "Verification");
				break;
				
			case CF_CHECK_MEAS:															
				strcpy(pszEventType, "Cf Check    ");
				break;
				
			case CF_NORM_MEAS:															
				strcpy(pszEventType, "Cf Norm     ");
				break;
				
			case TOTALS_MEAS:															
				strcpy(pszEventType, "Totals      ");
				break;
			
			case MII_MEAS:															
				strcpy(pszEventType, "GRAND MII   ");
				break;
									
			default:															
				strcpy(pszEventType, "Unknown     ");
				break;
			}	
			break;
			
		default:
			strcpy(pszEventType, "Unknown     ");
			break;
	}		
}

void GetEventType1Str(struct db_event_rec *pdbEvent, char *pszEventType)
{

	switch (pdbEvent->us_event_type) {	// event type
		case NOT_RECONCILED_EVENT:
			strcpy(pszEventType, "Unreconciled");
			break;
																					
		case RECONCILED_EVENT:						
			strcpy(pszEventType, "Reconciled  ");
			break;
																					
		case UNEXPLAINED_EVENT:
			strcpy(pszEventType, "Unexplained ");
			break;
																					
		case MISSING_DATA_EVENT:
			strcpy(pszEventType, "Missing Data");
			break;
			
		case CHANNEL_EVENT:
			strcpy(pszEventType, "Channel     ");															
			break;
			
		case BACKGROUND_EVENT:
			strcpy(pszEventType, "Background  ");
			break;

		case MEASUREMENT_EVENT:
			strcpy(pszEventType, "Measurement ");
			break;
			
		default:
			strcpy(pszEventType, "Unknown     ");
			break;
	}		
}

void GetEventType2Str(struct db_event_rec *pdbEvent, char *pszEventType)
{

	switch (pdbEvent->us_event_type) {	// event type
		case NOT_RECONCILED_EVENT:
			strcpy(pszEventType, "Unknown     ");
			break;
																					
		case RECONCILED_EVENT:						
			strcpy(pszEventType, "Unknown     ");
			break;
																					
		case UNEXPLAINED_EVENT:
			strcpy(pszEventType, "Unknown     ");
			break;
																					
		case MISSING_DATA_EVENT:
			strcpy(pszEventType, "Unknown     ");
			break;
			
		case CHANNEL_EVENT:
			strcpy(pszEventType, "Unknown     ");															
			break;
			
		case BACKGROUND_EVENT:
			switch (pdbEvent->us_event_type1) {
			case LOW_BKG_MEAS:															
				strcpy(pszEventType, "Low Bkg     ");
				break;
																						
			case HIGH_BKG_MEAS:															
				strcpy(pszEventType, "High Bkg    ");
				break;

			default :
				strcpy(pszEventType, "Background  ");															
				break;
			}
			break;

		case MEASUREMENT_EVENT:
			switch (pdbEvent->us_event_type1) {							// measurement type
			case EMPTY_MEAS:															
				strcpy(pszEventType, "Empty       ");
				break;
																						
			case ASSAY_MEAS:															
				strcpy(pszEventType, "Verification");
				break;
				
			case CF_CHECK_MEAS:															
				strcpy(pszEventType, "Cf Check    ");
				break;
				
			case CF_NORM_MEAS:															
				strcpy(pszEventType, "Cf Norm     ");
				break;
				
			case TOTALS_MEAS:															
				strcpy(pszEventType, "Totals      ");
				break;
			
			case MII_MEAS:															
				strcpy(pszEventType, "GRAND MII   ");
				break;
									
			default:															
				strcpy(pszEventType, "Unknown     ");
				break;
			}	
			break;
			
		default:
			strcpy(pszEventType, "Unknown     ");
			break;
	}		
}
	

///////////////////////////////////////////////////////////////////////////
//	Name:	GetDirEventStatusStr
//
//	Description:
//  Return a string based on the Direction event status parameter 
//
//	Declaration:
//	static void GetDirEventStatusStr(struct db_dir_event_rec *pdbEvent, char *pszEventStatus)
//
//	Input:	pdbEvent	event of interest
//			
//	Output:	none
//
//	Return:	string with type identifier
//	
//  date    /	author	revision
//  -----------------	--------
//	15-Dec2004	SFK		Created modeled on GetDirEventTypeStr
//////////////////////////////////////////////////////////////////
void GetDirEventStatusStr(struct db_dir_event_rec *pdbDirEvent, char *pszEventStatus)
{

	switch (pdbDirEvent->us_dir_event_status) {						// event status
		case NOT_RECONCILED_EVENT:
			strcpy(pszEventStatus, "Unreconciled");
			break;

		case RECONCILED_EVENT:
			strcpy(pszEventStatus, "Reconciled  ");
			break;

		case UNEXPLAINED_EVENT:
			strcpy(pszEventStatus, "Unexplained ");
			break;

		case MISSING_DATA_EVENT:
			strcpy(pszEventStatus, "Missing Data");
			break;

		case CHANNEL_EVENT:
			strcpy(pszEventStatus, "Channel     ");
			break;
			
		default:
			strcpy(pszEventStatus, "Unknown     ");
			break;
	}
}