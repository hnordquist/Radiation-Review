///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001-2005, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : DlgExport.cpp     
//Primary Author: Shirley Klosterbuer

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

//19 Apr 2006 - pjm - changed code to reduce compiler warnings.

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//		ExportToINCCMenuOption() - display dialog and do action
//		ExportToINCCDialogBox() - reads parameters and displays first dialog only
//		ExportToINCCDoIt() - reads parameters and performs first dialog actions
//		CleanUpExportToINCC() - cleans up any allocated lists from either dialog box
//		static EvaluateEvent() - checks whether event matches conditions
//		static CountMatchingEvents() - counts number of events that match conditions
//		static WriteDefaultDbParameters() - writes defaults for first dialog
//		static UpdateDbParms() - asks question followed by write defaults for first dialog
//		static ReadDefaultDbParameters() - reads defaults for first dialog
//		static CleanUpExportOptionsDialogBox() - cleans up allocations from first dialog box
//		static PrintMeasEventToTextDBFile() - writes file for IR
//		static SelectAllSta() - selects all station checkboxes on first dialog
//		static DeselectAllSta() - de-selects all station checkboxes on first dialog
//		static AllocateTableDialogBox() - creates all lists for second dialog box
//		static CleanUpExportTableDialogBox() - de-allocates all lists in second dialog box
//		static UpdateSingleEventInDb() - updates edited event in row in db from table dialog box
//		static UpdateAllEventsInDb() - updates events in db for all rows in table dialog box
//		static PutThisEventInTableRow() - creates one row in the table
//		static ReadMeasurementsFromDbByTime() - reads all measurements from db based on time
//		static ReadMeasurementsFromDbByStation() - reads all measurements from db based on station
//		static AccessIdFile() - read item ID information from the file created by Operator Review
//		static SelectAll() - selects all rows in the second dialog box (the table)
//		static DeselectAll() - de-selects all rows in the second dialog box (the table)
//		static ProcessTableEdit() - keep track of whether user has edited the table in second dialog box
//		static RereadDbAndUpdateTable() - if graph is linked in, re-read events and update table when graph event saved
//		static LinkToGraph() - sets up link to graph, registers function for when edit events on graph
//		static ExportTableDialogBox() - displays the second dialog box (the table of events to write)
//		static DoIt() - actions of second dialog box (writes selected events to INCC and IR file)
//		static ExportOptionsDialogBox() - displays first dialog box
///////////////////////////////////////////////////////////////////////////

#include "gui_tbl.h"          
#include "RAD.H"                        
#include "DateTimeUtils.h"
#include "DbDefault.h"
#include "DbEvent.h"
#include "DbGeneralDbSupport.h"
#include "DbOperator.h"
#include "EventTypeLists.h"
#include "FacilityConfig.h"
#include "Interval.h"
#include "Plot.h"
#include "PrintFacilityHeader.h"
#include "RadInit.h"  
#include "upperlayer/RTComs.h"	// 8-Dec-04 SFK Added relative path
#include "upperlayer/TXTWRDB.H"	// 8-Dec-04 SFK Added relative path
#include "Utilities.h"
#include "WriteNCCFile.h"   
#include "WriteXML.h"
#include "MyDateTime.h"
#include "TypeDefinitions.h"
#include <malloc.h>

#define ENTER_ID "Enter ID"
#define NO_ID "None"

#define EXPORT_ASSAY_BIT		0x0001
#define EXPORT_CF_BIT			0x0002
#define EXPORT_NORM_BKG_BIT		0x0004
#define EXPORT_EMPTY_ASSAY_BIT	0x0008
#define EXPORT_TOTALS_ASSAY_BIT	0x0010
#define EXPORT_CHANNEL_BIT		0x0020		// added 13-Dec-04 AFK


extern RADInit *pRAD_Init;  
extern char *glnaszIntervalUnitsPicklist[];
extern char *glnaszDirectionList[];
extern short glsFacNum;
extern CMyDateTime glMyDateTime;
extern char glszOldDbName[];

//	1/3/2005 SFK Added flag bits to track what files to write
#define NCC_BIT	1
#define IR_BIT	2
#define XML_BIT	4
static BOOL gllbNCCFile = FALSE;
static BOOL gllbIRFile = FALSE;
static BOOL gllbXMLFile = FALSE;

static CGUI_TextWin *pWin = NULL;     
static short gllsFacNum;
//static short gllsSta = 0;
static IDStruct gllStaID;
static char **gllaszStaList = NULL;	// picklist of stations in this facility
static short gllsMaxStaNameLen = 0;
// parameters need for stations table
static int glliNumStas = -1;    
static BOOL *pgllbSelectTableList = NULL;
static BOOL *pgllbStaSelect = NULL;
static unsigned short gllusMeasType = 0;
static unsigned long gllulTolerance = 300;
static unsigned long gllulUserInterval = 0;
static DATE glldStart = 0.0;
static DATE glldEnd = 0.0;
static unsigned int glluiUnits = 0;                 
static char  gllszFirstDbDate[DT_LEN+1], gllszLastDbDate[DT_LEN+1];              
static unsigned long gllulDbIntervalInDays = -1;
static struct GEN_DATE_STRUCT gllGenDate;
static struct GEN_TIME_STRUCT gllGenTime;
static char **gllnaszTypeList = NULL;
// parameters needed for measurement table
static CGUI_Dlg *gll_pDlg = NULL;
static BOOL gllbEditsInMeasurementTable;
static BOOL gllbRealTimeUpdates = FALSE;
static BOOL gllbLinked = FALSE;
static int glliNumMatchingEvents = -1;    
static int glliPrevNumMatchingEvents = -1;
static unsigned int glluiCheckBoxCol;
static unsigned int glluiIDCol;
static unsigned int glluiTypePickListCol;
static unsigned int glluiDateCol;
static unsigned int glluiStaCol;
static unsigned int glluiStartTimeCol;
static unsigned int glluiEndTimeCol;
static unsigned int gllusStaCol;   

static char **pgllszIDTableList = NULL;
static char (*gllszID)[ID_LEN+1] = NULL;

static unsigned int *pglluiID = NULL;
static unsigned int *pglluiTypeTableList = NULL;

static short *pgllsSta = NULL;
static short *pgllsStaType = NULL;
static short *pgllsChan = NULL;

static char **pgllszStaNameTableList = NULL;
static char (*gllszSta)[DB_NAME_LEN+1] = NULL;

static char **pgllDateTableList = NULL;
static char (*gllszDate)[DT_LEN+1] = NULL;
static char (*gllszEndDate)[DT_LEN+1] = NULL;

static char **pgllStartTimeTableList = NULL;
static char (*gllszStartTime)[DT_LEN+1] = NULL;

static char **pgllEndTimeTableList = NULL;
static char (*gllszEndTime)[DT_LEN+1] = NULL;

//static char **pgllEventTypesTableList = NULL;		// 13-Dec-2004 Add list of pointers to drop down lists

static DATE *pglldOrgBeg = NULL;
static DATE *pglldOrgEnd = NULL;
static Text_Write_Database *gllpWriteDB = NULL;

static BOOL EvaluateEvent(short sStaIndex, struct db_event_rec *pdbChanEvent);
static void CountMatchingEvents(int *piNumMatches);
static void WriteDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL ReadDefaultDbParameters();
static void CleanUpExportOptionsDialogBox();
static BOOL PrintMeasEventToTextDBFile(struct db_event_rec *pdbEvent, unsigned long ulNumPrinted, unsigned long ulTotalRecs, unsigned short *pusDir);
static void SelectAllSta(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void DeselectAllSta(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL AllocateTableDialogBox();
static void CleanUpExportTableDialogBox();
static void UpdateSingleEventInDb(int iRow);
static void UpdateAllEventsInDb();
static BOOL PutThisEventInTableRow(struct db_event_rec *pdbChanEvent, int *piRow, int iMaxRows);
static BOOL ReadMeasurementsFromDbByTime();
static BOOL ReadMeasurementsFromDbByStation();
static BOOL AccessIdFile(int iAction, char **pIDs[], short *psNumIDs);
static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL ProcessTableEdit(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol);
static void RereadDbAndUpdateTable();
static void LinkToGraph(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
static BOOL ExportTableDialogBox(BOOL bAutomated);
static BOOL CloseTextWindow(CGUI_TextWin *pWinArg);
static BOOL DoIt();
static BOOL ExportOptionsDialogBox(BOOL bAutomated);

/*===========================================================================
 *
 *  Name	 :  ExportToINCCMenuOption
 *
 *  Purpose	 :  Called from the main menu to execute both the dialog box and
 *				to perform the action.
 *
 *  Return value :  TRUE/FALSE
 *
 *  Implicit Input : 
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK		Created
 *	08-Aug-2002	SFK		Changed to DATE
 *	13-Dec-2004	SFK		Modified messages to not refer to INCC
 *
===========================================================================*/
BOOL ExportToINCCMenuOption()
{
	BOOL bDum;
	GUI_ACTION response;
	CGFmtStr Msg;

	bDum = ExportOptionsDialogBox(FALSE);
	if (bDum) bDum = ExportTableDialogBox(FALSE);
	if (bDum) DoIt();
	if (bDum) {
		if (pRAD_Init->Get_ReanalyzeEnabled() == TRUE) {
			Msg.Printf("Export of data finished.");
			Msg.PrintfAppend("\nIf using Integrated Review, a Reanalyze will be performed.");
			Msg.PrintfAppend("\nAre you using Integrated Review now?");
			response = GUI_MsgBox(Msg, GUI_ICON_QUESTION, GUI_YESNO);
			if (response == GUI_NO) bDum = FALSE;
		}
		else {
			GUI_MsgBox("Export of data finished.", GUI_ICON_INFO);
		}
	}
	return(bDum);
}	


///////////////////////////////////////////////////////////////////////////////////////////////
//	Perform only the dialog box setup 
//	Called when started with /SETUP
//	08-Aug-2002	SFK		Changed to DATE
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL ExportToINCCDialogBox()
{    
	ReadDefaultDbParameters();
	GetReviewPeriodParameters(&glldStart, &glldEnd, &gllulDbIntervalInDays);
	glluiUnits = 0;
	return(ExportOptionsDialogBox(TRUE));
}	
    
    
///////////////////////////////////////////////////////////////////////////////////////////////////
//	Perform just the export to INCC
//	Called when started with /IMPORT
//	08-Aug-2002	SFK		Changed to DATE
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL ExportToINCCDoIt()
{  
	BOOL bDum;

	ReadDefaultDbParameters();	// get which stations are selected for export
	GetReviewPeriodParameters(&glldStart, &glldEnd, &gllulDbIntervalInDays);
	glluiUnits = 0;
	bDum = ExportTableDialogBox(FALSE);
	if (bDum) DoIt();
	return(bDum);
}	



// clean up everything about the export option dialog boxes
void CleanUpExportToINCC()
{
	CleanUpExportOptionsDialogBox();
	CleanUpExportTableDialogBox();
}



	
/*===========================================================================
 *
 *  Name	 :	EvaluateEvent
 *
 *  Purpose	 :  Based on the station and gllusMeasType, determine whether
 *				the present event meets the conditions.
 *
 *  Return value :  TRUE - event should be processed
 *					FALSE - event not on channel or station wanted or does
 *						not match type of events requested.
 *
 *  Special notes: pdbChanEvent->us_event_type1 should be one of the following (from raddefs.h)
 *	#define UNKNOWN_MEAS		0		// a type of MEASUREMENT_EVENT
 *	#define LOW_BKG_MEAS		1		// a type of BACKGROUND_EVENT
 *	#define HIGH_BKG_MEAS		2		// a type of BACKGROUND_EVENT
 *	#define EMPTY_MEAS			3		// a type of MEASUREMENT_EVENT
 *	#define ASSAY_MEAS			4		// a type of MEASUREMENT_EVENT
 *	#define CF_CHECK_MEAS		5		// a type of MEASUREMENT_EVENT
 *	#define CF_NORM_MEAS		6		// a type of MEASUREMENT_EVENT
 *	#define NORMAL_BKG_MEAS		7		// a type of BACKGROUND_EVENT
 *	#define TOTALS_MEAS			8		// a type of MEASUREMENT_EVENT
 *	#define MII_MEAS			9
 *
 *
 *  Implicit Input :	gllusMeasType tells which types of measurement are requested
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK		Created
 *	08-Aug-2002	SFK		Changed to DATE
 *  29-Aug-2002	SFK		Patch where assume channel number is always 1 greater than offset
 *	13-Dec-2004	SFK		Add check for station type and take appropriate action whether
 *						GRAND or SR
 *
===========================================================================*/
static BOOL EvaluateEvent(short sStaIndex, struct db_event_rec *pdbChanEvent)
{
	BOOL bMeasValid = FALSE;
	BOOL bStaOk = FALSE;
	short sChan;
	struct IDStruct StaID;

	// check if the event is associated with the station of the picklist choice
	if (!pglFacCfg->GetStationIDForPicklistChoice(sStaIndex, &StaID)) return FALSE;
	if ((pdbChanEvent->us_event_sta != StaID.sStaID) || (pdbChanEvent->us_event_sta_type != StaID.sStaType)) return(bMeasValid);

	// For normal measurements, only events on the Reals channel are used.
	// For totals measurements, only events on the Totals channel are used 
	if ((pdbChanEvent->us_event_sta_type == JSR_TYPE) || (pdbChanEvent->us_event_sta_type == JSR1_TYPE) 
		|| (pdbChanEvent->us_event_sta_type == ISR_TYPE)) {
		if (pdbChanEvent->us_event_type1 == TOTALS_MEAS) {
			sChan = TOTALS_OFFSET+1;
		}
		else {
			sChan = REALS_OFFSET+1;
		}
		if ((pdbChanEvent->us_event_chan != sChan) ||
			(pdbChanEvent->us_event_sta != StaID.sStaID) ||
			(pdbChanEvent->us_event_sta_type != StaID.sStaType)) return(FALSE);	// event does not belong to our channel


		// check whether the event should be displayed based on which types of events user has selected
		if ((gllusMeasType & EXPORT_ASSAY_BIT) == EXPORT_ASSAY_BIT) {
			if ((pdbChanEvent->us_event_type == MEASUREMENT_EVENT) && (pdbChanEvent->us_event_type1 == ASSAY_MEAS))
				bMeasValid = TRUE;
		}

		if ((gllusMeasType & EXPORT_CF_BIT) == EXPORT_CF_BIT) {
			if ((pdbChanEvent->us_event_type == MEASUREMENT_EVENT) && (pdbChanEvent->us_event_type1 == CF_NORM_MEAS))
				bMeasValid = TRUE;
		}

		if ((gllusMeasType & EXPORT_NORM_BKG_BIT) == EXPORT_NORM_BKG_BIT) {
			if ((pdbChanEvent->us_event_type == BACKGROUND_EVENT) && (pdbChanEvent->us_event_type1 == NORMAL_BKG_MEAS))
				bMeasValid = TRUE;
		}

		if ((gllusMeasType & EXPORT_EMPTY_ASSAY_BIT) == EXPORT_EMPTY_ASSAY_BIT) {
			if ((pdbChanEvent->us_event_type == MEASUREMENT_EVENT) && (pdbChanEvent->us_event_type1 == EMPTY_MEAS))
				bMeasValid = TRUE;
		}
		
		if ((gllusMeasType & EXPORT_TOTALS_ASSAY_BIT) == EXPORT_TOTALS_ASSAY_BIT) {
			if ((pdbChanEvent->us_event_type == MEASUREMENT_EVENT) && (pdbChanEvent->us_event_type1 == TOTALS_MEAS))
				bMeasValid = TRUE;
		}
	}

	if (pdbChanEvent->us_event_sta_type == GRAND_TYPE) {
		if ((gllusMeasType & EXPORT_CHANNEL_BIT) == EXPORT_CHANNEL_BIT)
			if (pdbChanEvent->us_event_type == CHANNEL_EVENT)	bMeasValid = TRUE;
		if ((gllusMeasType & EXPORT_NORM_BKG_BIT) == EXPORT_NORM_BKG_BIT)
			if ((pdbChanEvent->us_event_type == BACKGROUND_EVENT) && (pdbChanEvent->us_event_type1 == NORMAL_BKG_MEAS))	bMeasValid = TRUE;
	}

	return(bMeasValid);
}

/*===========================================================================
 *
 *  Name	 :	CountMatchingEvents
 *
 *  Purpose	 :  Go through all the possible stations and for selected stations
 *				count all events that match conditions.
 *
 *  Return value :  none
 *
 *  Special notes: 
 *
 *  Implicit Input : pgllbStaSelect tells whether station selected
 *					glliNumStas is total number of stations possible
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK	Created
 *	08-Aug-2002	SFK		Changed to DATE
 *
===========================================================================*/
static void CountMatchingEvents(int *piNumMatches)
{
	struct db_event_rec dbChanEvent;
	int status;
	int i;

	*piNumMatches = 0;

	for (i=0; i<glliNumStas; i++) {
		if (pgllbStaSelect[i] == TRUE) { // search through all the events on a station by station bases
			status = ScanChannelEvents(glldStart, FIRST, &dbChanEvent);
			do {
				if (dbChanEvent.d_event_beg_time_key < glldEnd) {
					if (EvaluateEvent(i, &dbChanEvent)) (*piNumMatches)++;
					status = ScanChannelEvents(glldStart, NEXT, &dbChanEvent);	// go on to next event
				}
			} while ((status ==TRUE) && (dbChanEvent.d_event_beg_time_key < glldEnd));
		}
	}
}


/*===========================================================================
 *
 *  Name	 :  WriteDefaultDbParameters, UpdateDbParms, ReadDefaultDbParameters
 *
 *  Purpose	 :  Called from the dialog box when the NewDefaults button is pushed.
 *				The present values are read from the dialog box and written to
 *				the Default database.
 *
 *  Return value :  None
 *
 *  Special notes:  User is notified if a problem is encountered in updating
 *					the default database
 *
 *  Implicit Input : 
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK		Created
 *	08-Aug-2002	SFK		Changed to DATE
 *	13-Dec-2004	SFK		Added EXPORT_CHANNEL stuff
 *
===========================================================================*/
static void WriteDefaultDbParameters(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	int status;   
	short sNum;
	CGStr Str;
	BOOL bExportAssay = FALSE;
	BOOL bExportCf = FALSE;
	BOOL bExportNormBkg = FALSE;
	BOOL bExportEmptyAssay = FALSE;
	BOOL bExportTotalsAssay = FALSE;
	BOOL bExportChannel = FALSE;
	UINT uiNum = 0;

   /* ------------------------------------------------------------------
    *	Read the current table entry values and update the default
    *	parms in the db.
    * ----------------------------------------------------------------*/
	CGUI_Table *pTable;
    int i;
	struct db_sta_dflt_rec dbStaDflt;
	struct IDStruct TempID;
	pDlg->GetTable(IDC_EXPORT_PICKTABLE, &pTable);
	if (pTable != NULL) {	
		pTable->RetrieveAllCellValues();
	    for (i=0; i<glliNumStas; i++) {
			if (!pglFacCfg->GetStationIDForPicklistChoice(i, &TempID)) return;
			dbStaDflt.s_sta_dflt_num_key = TempID.sStaID;	// going to change one field so read the other fields
			dbStaDflt.s_sta_dflt_type = TempID.sStaType;
			status = AccessDefaultDatabase(READ, 0, &dbStaDflt); 
			if (status != TRUE) goto exit;
			dbStaDflt.b_export_select = pgllbStaSelect[i]; // update the field we know about here
			status = AccessDefaultDatabase(WRITE, 0, &dbStaDflt); 
			if (status != TRUE) goto exit;
	    }	
    }

   /* ------------------------------------------------------------------
    *	Get the current measurement type selected and write
    *	to default database.
    * ----------------------------------------------------------------*/ 
	pDlg->CheckBoxGetValue(IDC_EXPORT_ASSAY, &bExportAssay);
	pDlg->CheckBoxGetValue(IDC_EXPORT_CF, &bExportCf);
	pDlg->CheckBoxGetValue(IDC_EXPORT_NORM_BKG, &bExportNormBkg);
	pDlg->CheckBoxGetValue(IDC_EXPORT_EMPTY_ASSAY, &bExportEmptyAssay);
	pDlg->CheckBoxGetValue(IDC_EXPORT_TOTALS_ASSAY, &bExportTotalsAssay);
	pDlg->CheckBoxGetValue(IDC_EXPORT_CHANNEL, &bExportChannel);
	sNum = 0;
	if (bExportAssay)		sNum = sNum | EXPORT_ASSAY_BIT;
	if (bExportCf)			sNum = sNum | EXPORT_CF_BIT; 
	if (bExportNormBkg)		sNum = sNum | EXPORT_NORM_BKG_BIT;
	if (bExportEmptyAssay)	sNum = sNum | EXPORT_EMPTY_ASSAY_BIT;
	if (bExportTotalsAssay) sNum = sNum | EXPORT_TOTALS_ASSAY_BIT;
	if (bExportChannel)		sNum = sNum | EXPORT_CHANNEL_BIT;

    status = AccessDefaultDatabase(WRITE, S_EXPORT_MEASUREMENT_TYPE, &sNum);
	if (status != TRUE) goto exit;
    
   /* ------------------------------------------------------------------
    *	Write the changes to the .ini file
	*	1/3/2005 SFK	Changed to write flags for the 3 possible files
    * ----------------------------------------------------------------*/     
	
	BOOL bBool;
	pDlg->CheckBoxGetValue(IDC_EXPORT_SRCH_FILE, &bBool);
	if (bBool) uiNum = uiNum | IR_BIT;
	pDlg->CheckBoxGetValue(IDC_EXPORT_SRCH_FILE1, &bBool);
	if (bBool) uiNum = uiNum | XML_BIT;
	pDlg->CheckBoxGetValue(IDC_EXPORT_SRCH_FILE2, &bBool);
	if (bBool) uiNum = uiNum | NCC_BIT;
	pRAD_Init->Set_WriteMeasurementResults(uiNum);
   	pRAD_Init->Save_Parameters();

	SaveDatabaseNow();   
	return;
	                                                               
exit:
	RadReviewMsg(status, "when writing to Default database.  Update not completed.");	
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//	Ask question and then write default parameters
////////////////////////////////////////////////////////////////////////////////////////////////////
static void UpdateDbParms(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    int status;  
    
	status = AskUpdateQuestion();
	if (status != TRUE) return;
	WriteDefaultDbParameters(pDlg, CtrlID);
}        
                                                      
////////////////////////////////////////////////////////////////////////////////////////////////////
//	Read default parameters from database that are used in the first dialog box
////////////////////////////////////////////////////////////////////////////////////////////////////
static BOOL ReadDefaultDbParameters()
{
	int status;
	short sNum;
	int i;
	struct db_sta_dflt_rec dbStaDflt;

	if ((!AccessDefaultDatabase(READ, S_EXPORT_STATION, &gllStaID.sStaID)) ||
		(!AccessDefaultDatabase(READ, S_EXPORT_STATION_TYPE, &gllStaID.sStaType)) ||
		(!AccessDefaultDatabase(READ, S_EXPORT_MEASUREMENT_TYPE, &sNum))) {
		RadReviewMsg(uiDFLT_DB_READ_ERR, "Tools | Export Data.  Cannot proceed with menu option.");			
		return(FALSE);
	}

	// make sure you get a valid station from the db
	if (!pglFacCfg->IsStationEnabled(glsFacNum, &gllStaID)) {
		if (pglFacCfg->GetNumStations(glsFacNum) > 0) {
			pglFacCfg->GetFirstStationID(glsFacNum, &gllStaID);
		}
		else {
  			RadReviewMsg(uiNO_STATIONS_FOR_FACILITY_ERR);
			return(FALSE);
		}
	}
	gllusMeasType = sNum;		// bits for the measurement types checked

	short sNumStas = pglFacCfg->GetNumStations(glsFacNum);
	glliNumStas = sNumStas;
	if (sNumStas) {  //KM - If we have stations present (i.e. not zero stations)  
		int iTemp;
		if (!pglFacCfg->CreateStationPicklist(glsFacNum, false, &gllaszStaList, &iTemp)) return(FALSE);
		gllsMaxStaNameLen = (short)iTemp;
	} 	
	gllsMaxStaNameLen++;
	
   /* ------------------------------------------------------------------
    *	Allocate storage for all station select lists that will be in the station table
    * ----------------------------------------------------------------*/
	if (pgllbStaSelect) free(pgllbStaSelect);
    pgllbStaSelect = (BOOL *)malloc(glliNumStas*sizeof(BOOL));
    if (pgllbStaSelect == NULL) {
		pglFacCfg->DestroyPicklist(gllaszStaList);
		gllaszStaList = NULL;
	 	return(FALSE);
	} 	
		                             
   /* ------------------------------------------------------------------
    *	Fill in the default values for the table lists
    * ----------------------------------------------------------------*/
	struct IDStruct TempID;
	for (i=0; i<glliNumStas; i++) {
		if (!pglFacCfg->GetStationIDForPicklistChoice(i, &TempID)) return(FALSE);
		dbStaDflt.s_sta_dflt_num_key = TempID.sStaID;
		dbStaDflt.s_sta_dflt_type = TempID.sStaType;
		status = AccessDefaultDatabase(READ, 0, &dbStaDflt); // read station defaults from default db
		if (status != TRUE) {
			CleanUpExportToINCC();
		 	return(FALSE);
		} 	
		pgllbStaSelect[i] = dbStaDflt.b_export_select;
	}          
	
	//	1-3-2005	SFK		Read which files, if any, to write from RAD.INI
	unsigned int uiWriteFile = pRAD_Init->Get_WriteMeasurementResults();
	if (uiWriteFile & NCC_BIT) gllbNCCFile = TRUE; else gllbNCCFile = FALSE;
	if (uiWriteFile & IR_BIT) gllbIRFile = TRUE; else gllbIRFile = FALSE;
	if (uiWriteFile & XML_BIT) gllbXMLFile = TRUE; else gllbXMLFile = FALSE;
	
	return(TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// deallocate any memory used for the export options dialog box
////////////////////////////////////////////////////////////////////////////////////////////////////
static void CleanUpExportOptionsDialogBox()
{
	if (pgllbStaSelect)
		free(pgllbStaSelect);
	pgllbStaSelect = NULL;

 	if (gllaszStaList) gen_deallocate_str_array(gllaszStaList);
	gllaszStaList = NULL;
}

/*===========================================================================
 *
 *  Name	 :  PrintMeasEventToTextDBFile
 *
 *  Purpose	 :  Prints the measurement to the text file to be used by IR.
 *
 *  Return value :  TRUE/FALSE
 *
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK		Created
 *	08-Aug-2002	SFK		Changed to DATE
 *	13-Dec-2004	SFK		Check if SR data and then only write verification measurements
 *	15-Dec-2004	SFK		Fixed format of file written for IR
 *						Pick up status type from general lists in GetEventTypeStr
 *
===========================================================================*/

static char *ppszLabels[] = {"RAD-Event ID",
							 "RAD-Start Timestamp",
							 "RAD-End TimeStamp",
							 "RAD-Location (Detector)",
							 "RAD-Direction",
							 "RAD-Start Date/Time",
							 "RAD-End Date/Time",							 
							 "RAD-Start Date",
							 "RAD-Start Time",
							 "RAD-End Date",
							 "RAD-End Time",
							 "RAD-Inspector Entered Item ID",
							 "RAD-Status",
							 "RAD-1a",
							 "RAD-1b",
							 "RAD-2a",
							 "RAD-2b",
							 "RAD-3a",
							 "RAD-3b",
							 "RAD-4a",
							 "RAD-4b"};
							 // 21 columns 
							 // max column name length = RAD-Start Timestamp = 19 chars
							 // max column data length = timestamp 0000019970502090810000 = 22           

#define NUM_COLUMNS 21		// count columns in above labels
#define MAX_LABEL_LEN sizeof ("RAD-Inspector Entered Item ID") + 1 // length of longest label.

static BOOL PrintMeasEventToTextDBFile(struct db_event_rec *pdbEvent, unsigned long ulNumPrinted, unsigned long ulTotalRecs, unsigned short *pusDir)
{   
	struct db_dir_event_rec dbDirEvent;
	CGFmtStr Msg;
	int status; 
	BOOL bStatus, bMatchingDirEvent;
	char szStartDate[DT_LEN+1], szEndDate[DT_LEN+1];
	char szTempStr[STEVES_DT_LEN+1], szStartDateStr[STEVES_DT_LEN+1], szEndDateStr[STEVES_DT_LEN+1];
	char szStartTime[DT_LEN+1];
	char szEndTime[DT_LEN+1];
	char cSep;
	int i;
	BOOL bWrite;
	//CMyDateTime Date;
	struct IDStruct TempID;

	*pusDir = NO_DIRECTION;	// must return something for direction

	// Check to see if the .ini file indicates an IR file should be written now, if not just return.
	if (pRAD_Init->Get_WriteMeasurementResults() == FALSE) {
		return(TRUE);
	}

	// Only verification type events should be written to RAD.DB
	// 01-20-99 change to only not write types of events that are not measurements
	// 13-Dec-2004 - change to only write Verification if SR data
	bWrite = TRUE;
	if ((pdbEvent->us_event_sta_type == JSR_TYPE) || (pdbEvent->us_event_sta_type == JSR1_TYPE) 
		|| (pdbEvent->us_event_sta_type == ISR_TYPE)) {
		if (pdbEvent->us_event_type != MEASUREMENT_EVENT) bWrite = FALSE;
		if (bWrite == FALSE) return(TRUE); // do not write this file in RAD.DB
	}
	
	// Try to determine the direction for this event.
	status = FindDirEventAssociatedWithChanEvent(pdbEvent, &bMatchingDirEvent, &dbDirEvent);
	if (bMatchingDirEvent == TRUE) {
		*pusDir = dbDirEvent.us_dir_event_direction;
	}
	else {
		*pusDir = NO_DIRECTION;
	}

	//	Get station name
	char szStaName[DB_NAME_LEN+1];
	TempID.sStaID = pdbEvent->us_event_sta;
	TempID.sStaType = pdbEvent->us_event_sta_type;
	pglFacCfg->GetStationLongName(glsFacNum, &TempID, szStaName, DB_NAME_LEN);
	char szPathAndName[PATH_LEN+DOSNAME_LEN+1];

	// If this is the first record to be recorded, create the object and write the header.
 	if (ulNumPrinted == 0) {
		// 3.0.3.9 Added getting path from fac mgr
		//pglFacCfg->GetFacilityDirectory(glsFacNum, szPathAndName, PATH_LEN); 
		//if ((szPathAndName[strlen(szPathAndName)-1] != '\\')) strcat(szPathAndName, "\\");
		if (!GetDirectory(glsFacNum, DATA_DIR, szPathAndName, PATH_LEN+DOSNAME_LEN+1)) return(FALSE);
		MyStrnCat(szPathAndName, pRAD_Init->Get_Summary_Data_Filename(), DOSNAME_LEN);

    // 	if ((gllpWriteDB  = new Text_Write_Database(pRAD_Init->Get_Summary_Data_Filename(), NUM_COLUMNS,	(UINT)ulTotalRecs, (const char **) ppszLabels, MAX_LABEL_LEN, uiTIME_STAMP_LENGTH)) == NULL) return(FALSE);	// maximum column data length
		if ((gllpWriteDB  = new Text_Write_Database(szPathAndName, NUM_COLUMNS,	(UINT)ulTotalRecs, (const char **) ppszLabels, MAX_LABEL_LEN, uiTIME_STAMP_LENGTH)) == NULL) return(FALSE);	// maximum column data length
    	if (bStatus = gllpWriteDB->Database_Created() != TRUE) {
    		RadReviewMsg(uiFILE_WRITE_ERR, pRAD_Init->Get_Summary_Data_Filename());
    		return(FALSE);	
    	}	
    }
                  
	// separator is always tab for these files                  
    cSep = '\t';  
    
    // write sequence number
    Msg.Printf("%04ld%c", ulNumPrinted, cSep);

	/* write start date/time as a timestamp */
	glMyDateTime.DATETimestampToStevesTimestampStrings(pdbEvent->d_event_beg_time_key, szTempStr, szStartDateStr);
	Msg.PrintfAppend("%s%c", szTempStr, cSep);
														
	/* write start date/time as a timestamp */
	glMyDateTime.DATETimestampToStevesTimestampStrings(pdbEvent->d_event_end_time, szTempStr, szEndDateStr);
	Msg.PrintfAppend("%s%c", szTempStr, cSep);
	
	/* write location as the name of the station */
	//Msg.PrintfAppend("%s%c", dbSta.sz_sta_name,  cSep);
	Msg.PrintfAppend("%s%c", szStaName,  cSep);
	
	/* write direction as determined by finding a matching direction record*/
	Msg.PrintfAppend("%s%c", glnaszDirectionList[*pusDir],  cSep);
		                     
	/* write start/end date/time in format of yyyymmdd - xx:xx:xx yyyymmdd - xx:xx:xx yyyymmdd xx:xx:xx yyyymmdd xx:xx:xx */
	glMyDateTime.DATETimestampToDateTimeStrs(pdbEvent->d_event_beg_time_key, szStartDate, szStartTime, DTF_4DIGIT_YEAR);
	glMyDateTime.DATETimestampToDateTimeStrs(pdbEvent->d_event_end_time, szEndDate, szEndTime, DTF_4DIGIT_YEAR);
	Msg.PrintfAppend("%s%c%s%c", szStartDateStr, cSep, szEndDateStr,cSep);	
	Msg.PrintfAppend("%s%c%s%c%s%c%s%c", szStartDate, cSep, szStartTime, cSep, szEndDate, cSep, szEndTime,cSep);
	
	Msg.PrintfAppend("%s%c", pdbEvent->sz_event_id, cSep);
	/* write inspector id*/


	// write measurement type
  	char szEventTypeStr[MAX_LINE_LEN+1];
	GetEventTypeStr(pdbEvent, szEventTypeStr);
	if (pWin != NULL) Msg.PrintfAppend("%s%c", szEventTypeStr,cSep);	// 15-Dec-2004 SFK Added getting Event Str from GetEventTypeStr
	
	/* write pairs used to make decision */
	char szFromChanName[DB_NAME_LEN+1], szToChanName[DB_NAME_LEN+1];
	for (i=0; i<MAX_PAIRS; i++) {
		if (bMatchingDirEvent == TRUE) {
			if (dbDirEvent.us_dir_event_from_chan[i] != 65535) {
				TempID.sStaID = dbDirEvent.us_dir_event_from_sta[i];
				TempID.sStaType = dbDirEvent.us_dir_event_from_sta_type[i];
				TempID.sChanID = dbDirEvent.us_dir_event_from_chan[i];
				pglFacCfg->GetChannelLongName(glsFacNum, &TempID, szFromChanName, DB_NAME_LEN);
				TempID.sStaID = dbDirEvent.us_dir_event_to_sta[i];
				TempID.sStaType = dbDirEvent.us_dir_event_to_sta_type[i];
				TempID.sChanID = dbDirEvent.us_dir_event_to_chan[i];
				pglFacCfg->GetChannelLongName(glsFacNum, &TempID, szToChanName, DB_NAME_LEN);
				Msg.PrintfAppend("%s%c%s%c", szFromChanName, cSep, szToChanName, cSep);
			}	
			else {
				Msg.PrintfAppend("N/A%cN/A%c", cSep, cSep);
			}
		}
		else {
			Msg.PrintfAppend("N/A%cN/A%c", cSep, cSep);
		}
	}
	
	// actually write to the file
	bStatus = gllpWriteDB->Row_Add(Msg);
	if (bStatus != TRUE) {
		RadReviewMsg(uiFILE_WRITE_ERR, pRAD_Init->Get_Summary_Data_Filename());
		return(FALSE);
	}
	return(TRUE); 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Action to put check in each station select box when user presses SelectAll key
////////////////////////////////////////////////////////////////////////////////////////////////////
static void SelectAllSta(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGUI_Table *pTable;
    int i;
    
	pDlg->GetTable(IDC_EXPORT_PICKTABLE, &pTable);
	if (pTable != NULL) {
	    for (i=0; i<glliNumStas; i++) {
	    	pgllbStaSelect[i] = TRUE;
	    }	
        pTable->UpdateCol(glluiCheckBoxCol);
    }
}        

////////////////////////////////////////////////////////////////////////////////////////////////////
// Action to remove check in each station select box when user presses DeselectAll key
////////////////////////////////////////////////////////////////////////////////////////////////////
static void DeselectAllSta(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGUI_Table *pTable;
	int i;
		
	pDlg->GetTable(IDC_EXPORT_PICKTABLE, &pTable);
	if (pTable != NULL) {
	    for (i=0; i<glliNumStas; i++) {
	    	pgllbStaSelect[i] = FALSE;
	    }	
        pTable->UpdateCol(glluiCheckBoxCol);
    }
}        




////////////////////////////////////////////////////////////////////////////////////////////////////
// allocate storage for everything used in the Export Table Dialog box
////////////////////////////////////////////////////////////////////////////////////////////////////
static BOOL AllocateTableDialogBox()
{
   /* ------------------------------------------------------------------
    *	Based on number of matching measurements, allocate storage for the 
    *	lists to be generated and for the lists of pointers to the strings.
    *	In case something has already been allocated, release the old
    *	before allocating the new.
    * ----------------------------------------------------------------*/            
	if (pgllbSelectTableList) free(pgllbSelectTableList);
	pgllbSelectTableList = (BOOL *)malloc(glliNumMatchingEvents*sizeof(BOOL));
	if (pgllbSelectTableList == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (pgllszIDTableList) free(pgllszIDTableList);                                         
    pgllszIDTableList =(char **)malloc(glliNumMatchingEvents*sizeof(char *));
	if (pgllszIDTableList == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (gllszID) free(gllszID);
	gllszID = (char (*)[ID_LEN+1])malloc(glliNumMatchingEvents*(ID_LEN+1));
	if (gllszID == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (pglluiID) free(pglluiID);                                         
    pglluiID =(unsigned int *)malloc(glliNumMatchingEvents*sizeof(unsigned int));
	if (pglluiID == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (pglluiTypeTableList) free(pglluiTypeTableList);                                         
    pglluiTypeTableList =(unsigned int *)malloc(glliNumMatchingEvents*sizeof(unsigned int));
	if (pglluiTypeTableList == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}
	
	if (pgllszStaNameTableList) free(pgllszStaNameTableList);                                         
    pgllszStaNameTableList =(char **)malloc(glliNumMatchingEvents*sizeof(char *));
	if (pgllszStaNameTableList == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (gllszSta) free(gllszSta);
	gllszSta = (char (*)[DB_NAME_LEN+1])malloc(glliNumMatchingEvents*(DB_NAME_LEN+1));
	if (gllszSta == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (pgllsSta) free(pgllsSta);
	pgllsSta = (short *)malloc(glliNumMatchingEvents*(sizeof(short)));
	if (pgllsSta == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}
	if (pgllsStaType) free(pgllsStaType);
	pgllsStaType = (short *)malloc(glliNumMatchingEvents*(sizeof(short)));
	if (pgllsStaType == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}
	if (pgllsChan) free(pgllsChan);
	pgllsChan = (short *)malloc(glliNumMatchingEvents*(sizeof(short)));
	if (pgllsChan == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (pgllDateTableList) free(pgllDateTableList);                                         
    pgllDateTableList =(char **)malloc(glliNumMatchingEvents*sizeof(char *));
	if (pgllDateTableList == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (pglldOrgBeg) free(pglldOrgBeg);
	pglldOrgBeg = (DATE *)malloc(glliNumMatchingEvents*(sizeof(DATE)));
	if (pglldOrgBeg == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (pglldOrgEnd) free(pglldOrgEnd);
	pglldOrgEnd = (DATE *)malloc(glliNumMatchingEvents*(sizeof(DATE)));
	if (pglldOrgEnd == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (gllszDate) free(gllszDate);
	gllszDate = (char (*)[DT_LEN+1])malloc(glliNumMatchingEvents*(DT_LEN+1));
	if (gllszDate == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (gllszEndDate) free(gllszEndDate);
	gllszEndDate = (char (*)[DT_LEN+1])malloc(glliNumMatchingEvents*(DT_LEN+1));
	if (gllszEndDate == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (pgllStartTimeTableList) free(pgllStartTimeTableList);                                         
    pgllStartTimeTableList =(char **)malloc(glliNumMatchingEvents*sizeof(char *));
	if (pgllStartTimeTableList == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (gllszStartTime) free(gllszStartTime);
	gllszStartTime = (char (*)[DT_LEN+1])malloc(glliNumMatchingEvents*(DT_LEN+1));
	if (gllszStartTime == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (pgllEndTimeTableList) free(pgllEndTimeTableList);                                         
    pgllEndTimeTableList =(char **)malloc(glliNumMatchingEvents*sizeof(char *));
	if (pgllEndTimeTableList == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	if (gllszEndTime) free(gllszEndTime);
	gllszEndTime = (char (*)[DT_LEN+1])malloc(glliNumMatchingEvents*(DT_LEN+1));
	if (gllszEndTime == NULL) {
	    CleanUpExportToINCC();
		return(FALSE);
	}

	return(TRUE);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// deallocate any memory used for the Export Table Dialog Box
////////////////////////////////////////////////////////////////////////////////////////////////////
static void CleanUpExportTableDialogBox()
{
	if (pgllbSelectTableList) free(pgllbSelectTableList);
	pgllbSelectTableList = NULL;

	if (pgllszIDTableList) free(pgllszIDTableList);                                         
    pgllszIDTableList = NULL;
	if (gllszID) free(gllszID);
	gllszID = NULL;

	if (pglluiID) free(pglluiID);                                         
    pglluiID = NULL;
	
	if (pglluiTypeTableList) free(pglluiTypeTableList);                                         
    pglluiTypeTableList = NULL;
	
	if (pgllszStaNameTableList) free(pgllszStaNameTableList);                                         
    pgllszStaNameTableList = NULL;
	if (gllszSta) free(gllszSta);
	gllszSta = NULL;
	if (pgllsSta) free(pgllsSta);
	pgllsSta = NULL;
	if (pgllsStaType) free(pgllsStaType);
	pgllsStaType = NULL;
	if (pgllsChan) free(pgllsChan);
	pgllsChan = NULL;

	if (pgllDateTableList) free(pgllDateTableList);                                         
    pgllDateTableList = NULL;
	if (gllszDate) free(gllszDate);
	gllszDate = NULL;
	if (gllszEndDate) free(gllszEndDate);
	gllszEndDate = NULL;

	if (pglldOrgBeg) free(pglldOrgBeg);
	pglldOrgBeg = NULL;
	if (pglldOrgEnd) free(pglldOrgEnd);
	pglldOrgEnd = NULL;
	
	if (pgllStartTimeTableList) free(pgllStartTimeTableList);                                         
    pgllStartTimeTableList = NULL;
	if (gllszStartTime) free(gllszStartTime);
	gllszStartTime = NULL;

	if (pgllEndTimeTableList) free(pgllEndTimeTableList);                                         
    pgllEndTimeTableList = NULL;
	if (gllszEndTime) free(gllszEndTime);
	gllszEndTime = NULL;

}


/*===========================================================================
 *
 *  Name	 :  	UpdateSingleEventInDb
 *					UpdateAllEventsInDb
 *
 *  Purpose	 :		UpdateSingleEventInDb -Based on information stored in table,
 *					update the db event specified by a single row in the table
 *					UpdateAllEventsInDb - Update all events in all rows
 *
 *  Return value :  None
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  30-Sep-1998 SFK		Created
 *	08-Aug-2002	SFK		Changed to DATE
 *	29-Aug-2002	SFK		Patch where assume channel number is always offset plus 1
 *	13-Dec=2004	SFK		Changed to set event type based on whether SR or GRAND type
 *						Added figuring out which dropdown list to use based on station type
===========================================================================*/
static void UpdateSingleEventInDb(int iRow)
{
	
	int status;
	struct db_event_rec dbChanEvent, dbOriginalDbChanEvent;
	short sChan;
	//CMyDateTime Date;

	// From information in the table figure out the beg and end time, handle case of end time in next day
	dbChanEvent.d_event_beg_time_key = glMyDateTime.DateTimeStrsToDATETimestamp(gllszDate[iRow], gllszStartTime[iRow]);
	dbChanEvent.d_event_end_time = glMyDateTime.DateTimeStrsToDATETimestamp(gllszEndDate[iRow], gllszEndTime[iRow]);

	// From what you have saved, figure out the channel, start and end time of the  original event
	dbChanEvent.us_event_chan = pgllsChan[iRow];
	dbChanEvent.us_event_sta = pgllsSta[iRow];
	dbChanEvent.us_event_sta_type = pgllsStaType[iRow];
	dbChanEvent.d_event_beg_time_key = pglldOrgBeg[iRow];
	dbChanEvent.d_event_end_time = pglldOrgEnd[iRow];

	// Read out the old record
	status = ReadEventRec(&dbChanEvent, &dbOriginalDbChanEvent);
	if (status != TRUE) {
		RadReviewMsg(uiEVENT_NOT_FOUND_WARN);
		return;
	}

	// Copy the ID from the table value to the event record read from the db
	MyStrnCpy(dbOriginalDbChanEvent.sz_event_id, gllszID[iRow], ID_LEN+1);

	// add code to update start and end time also if allow them to edit that in the table

	//	Based on the picklist choice in the table, update the us_event_type and us_event_type1
	//		part of the record.
	if ((pgllsStaType[iRow] == JSR_TYPE) || (pgllsStaType[iRow] == JSR1_TYPE) || (pgllsStaType[iRow] == ISR_TYPE)) {
		if (pglluiTypeTableList[iRow] == EXPORT_ASSAY_PL_CHC) {
			if ((dbOriginalDbChanEvent.us_event_type1 != ASSAY_MEAS) &&
				(dbOriginalDbChanEvent.us_event_type1 != EMPTY_MEAS)) { //don't overright empty, etc.
				dbOriginalDbChanEvent.us_event_type1 = ASSAY_MEAS;
				dbOriginalDbChanEvent.us_event_type = MEASUREMENT_EVENT;
			}
		}
		if (pglluiTypeTableList[iRow] == EXPORT_CF_PL_CHC) {
			if ((dbOriginalDbChanEvent.us_event_type1 != CF_CHECK_MEAS) &&
				(dbOriginalDbChanEvent.us_event_type1 != CF_NORM_MEAS)) { //don't overright either norm. type
				dbOriginalDbChanEvent.us_event_type1 = CF_NORM_MEAS;
				dbOriginalDbChanEvent.us_event_type = MEASUREMENT_EVENT;
			}
		}
		if (pglluiTypeTableList[iRow] == EXPORT_NORM_BKG_PL_CHC) {
			dbOriginalDbChanEvent.us_event_type1 = NORMAL_BKG_MEAS;
			dbOriginalDbChanEvent.us_event_type = BACKGROUND_EVENT;
		}
		if (pglluiTypeTableList[iRow] == EXPORT_TOTALS_ASSAY_PL_CHC) {
			dbOriginalDbChanEvent.us_event_type1 = TOTALS_MEAS;
			dbOriginalDbChanEvent.us_event_type = MEASUREMENT_EVENT;
		}
		// in case you changed from type, make sure you have the correct
		// event channel number now.
		if (dbOriginalDbChanEvent.us_event_type1 == TOTALS_MEAS) {
			sChan = TOTALS_OFFSET+1;
		}
		else {
			sChan = REALS_OFFSET+1;
		}
	}

	if (pgllsStaType[iRow] == GRAND_TYPE) {
		if (pglluiTypeTableList[iRow] == GRAND_CHAN_PL_CHC) {
			dbOriginalDbChanEvent.us_event_type = CHANNEL_EVENT;
			dbOriginalDbChanEvent.us_event_type1 = UNKNOWN_MEAS;
		}
		if (pglluiTypeTableList[iRow] == GRAND_BKGD_PL_CHC) {
			dbOriginalDbChanEvent.us_event_type = BACKGROUND_EVENT;
			dbOriginalDbChanEvent.us_event_type1 = NORMAL_BKG_MEAS;
		}
		sChan = dbChanEvent.us_event_chan;
	}

	//	13-Dec-2004	Get right dropdown list for this station type
	struct IDStruct TempID;
	short sDum;
	TempID.sChanID = dbChanEvent.us_event_chan;
	TempID.sStaID = dbChanEvent.us_event_sta;
	TempID.sStaType = dbChanEvent.us_event_sta_type;
	GetEventTypePicklist(TempID, EXPORT_LIST, &gllnaszTypeList, &sDum);
	


	dbOriginalDbChanEvent.us_event_chan = sChan;
	dbOriginalDbChanEvent.us_event_sta = dbChanEvent.us_event_sta;
	dbOriginalDbChanEvent.us_event_sta_type = dbChanEvent.us_event_sta_type;
	pgllsChan[iRow] = sChan;
	pgllsSta[iRow] = dbChanEvent.us_event_sta;
	pgllsStaType[iRow] = dbChanEvent.us_event_sta_type;

	// delete the original event and write in the new one
	// note: if allow user to edit the starting and ending date and time then use ReplaceEvent.
	DeleteDataByChannelEvents(&dbChanEvent);
//	struct IDStruct TempID;
	TempID.sStaID = dbOriginalDbChanEvent.us_event_sta;
	TempID.sStaType = dbOriginalDbChanEvent.us_event_sta_type;
	TempID.sStaType = dbOriginalDbChanEvent.us_event_chan;
	CreateChanEventRec(TempID, &dbOriginalDbChanEvent);
}

//	This assumes all the information has been retrieved from the table.
//	Retrieve the information from the table and write an updated event to the database
static void UpdateAllEventsInDb()
{
	int i;

	for (i=0; i<glliNumMatchingEvents; i++) {
		UpdateSingleEventInDb(i);
	}
}


/*===========================================================================
 *
 *  Name	 :  	PutThisEventInTableRow
 *
 *  Purpose	 :		For the specified event, fill in the appropriate row
 *					in the lists for a table entry
 *
 *  Return value :  TRUE - event entered in row
 *					FALSE - invalid row requested
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  30-Sep-1998 SFK		Created
 *	08-Aug-2002	SFK		Changed to DATE
 *	13-Dec-2004	SFK		Determine event type based on station type.
 *						Add putting in proper dropdown list for this station
 ===========================================================================*/
static BOOL PutThisEventInTableRow(struct db_event_rec *pdbChanEvent, int *piRow, int iMaxRows)
{
	int i;
	int status;
	struct db_oper_rec dbOp;
	//CMyDateTime Date;

	if (*piRow >= iMaxRows) return(FALSE); //invalid index requested
	i = *piRow;

	// always select all the measurements
	pgllbSelectTableList[i] = TRUE;

	// copy the ID from the event record
	////// **************** Try to find a matching ID in the operator list
	//pjm 25 Jan 2006 Got rid of compiler warning with explicit cast to  unsigned long
	unsigned long ulEventBegTimeKey = (unsigned long)glMyDateTime.DATETimestampToMyTimestamp(pdbChanEvent->d_event_beg_time_key);
	unsigned long ulEventEndTime = (unsigned long)glMyDateTime.DATETimestampToMyTimestamp(pdbChanEvent->d_event_end_time);
	dbOp.ul_op_id_time = ulEventBegTimeKey;
	dbOp.s_op_sta_num_key = pdbChanEvent->us_event_sta; 
	status = ReadOperatorRec(&dbOp, ulEventEndTime, gllulTolerance);
	if (status == TRUE) {
		strcpy(gllszID[i],dbOp.sz_op_item_id);
	}
	else {
		strcpy(gllszID[i],pdbChanEvent->sz_event_id);
	}
	pgllszIDTableList[i] = gllszID[i];	// set up the pointer for the table

	// convert the date and times to strings, and fill
	pglldOrgBeg[i] = pdbChanEvent->d_event_beg_time_key;  // will need when updating db
	glMyDateTime.DATETimestampToDateTimeStrs(pdbChanEvent->d_event_beg_time_key, gllszDate[i], gllszStartTime[i]);
	pgllDateTableList[i] = gllszDate[i];	// set up the pointer for the table
	pgllStartTimeTableList[i] = gllszStartTime[i];	// set up the pointer for the table
	
	pglldOrgEnd[i] = pdbChanEvent->d_event_end_time;  // will need when updating db
	glMyDateTime.DATETimestampToDateTimeStrs(pdbChanEvent->d_event_end_time, gllszEndDate[i], gllszEndTime[i]);
	pgllEndTimeTableList[i] = gllszEndTime[i];	// set up the pointer for the table

	if ((pdbChanEvent->us_event_sta_type == JSR_TYPE) || (pdbChanEvent->us_event_sta_type == JSR1_TYPE) ||
		(pdbChanEvent->us_event_sta_type == ISR_TYPE)) {
		// based on the measurement type, set the choice in the table pulldown picklist
		switch (pdbChanEvent->us_event_type1) {
			case EMPTY_MEAS:
				pglluiTypeTableList[i] = EXPORT_EMPTY_ASSAY_PL_CHC;
				break;

			case ASSAY_MEAS:
				pglluiTypeTableList[i] = EXPORT_ASSAY_PL_CHC;
				break;

			case CF_CHECK_MEAS:
				pglluiTypeTableList[i] = EXPORT_CF_PL_CHC;
				break;

			case CF_NORM_MEAS:
				pglluiTypeTableList[i] = EXPORT_CF_PL_CHC;
				break;

			case NORMAL_BKG_MEAS:
				pglluiTypeTableList[i] = EXPORT_NORM_BKG_PL_CHC;
				break;

			case TOTALS_MEAS:
				pglluiTypeTableList[i] = EXPORT_TOTALS_ASSAY_PL_CHC;
				break;

			default:
				pglluiTypeTableList[i] = EXPORT_ASSAY_PL_CHC;
				break;
		}
	}

	if (pdbChanEvent->us_event_sta_type == GRAND_TYPE) {
		switch (pdbChanEvent->us_event_type) {
			case CHAN_MEAS:
				pglluiTypeTableList[i] = GRAND_CHAN_PL_CHC;
				break;

			case NORMAL_BKG_MEAS:
				pglluiTypeTableList[i] = GRAND_BKGD_PL_CHC;
				break;

			default:
				pglluiTypeTableList[i] = GRAND_CHAN_PL_CHC;
				break;
		}
	}

	//	13-Dec-2004	Get right dropdown list for this station type
	struct IDStruct TempID;
	short sDum;
	static char **szTypeList = NULL;
	TempID.sChanID = pdbChanEvent->us_event_chan;
	TempID.sStaID = pdbChanEvent->us_event_sta;
	TempID.sStaType = pdbChanEvent->us_event_sta_type;
	GetEventTypePicklist(TempID, EXPORT_LIST, &gllnaszTypeList, &sDum);

	// Fill in the station name
//	struct IDStruct TempID;
	char szStaName[DB_NAME_LEN+1];
	TempID.sStaID = pdbChanEvent->us_event_sta;
	TempID.sStaType = pdbChanEvent->us_event_sta_type;
	pglFacCfg->GetStationLongName(glsFacNum, &TempID, szStaName, DB_NAME_LEN);
	strcpy(gllszSta[i],szStaName);
	pgllszStaNameTableList[i] = gllszSta[i];	// set up the pointer for the table

	// Remember channel number for later
	pgllsChan[i] = pdbChanEvent->us_event_chan;
	pgllsSta[i] = pdbChanEvent->us_event_sta;
	pgllsStaType[i] = pdbChanEvent->us_event_sta_type;

	// Fill in the id for the picklist
	pglluiID[i] = 1;

	// Increment the index counter by one
	(*piRow)++;
	return(TRUE);
}


/*===========================================================================
 *
 *  Name	 :  	ReadMeasurementsFromDbByTime
 *
 *  Purpose	 :		Read all the matching measurements from the database.
 *					Put them in the table sorted by time
 *
 *  Return value :  TRUE - all measurements read and entered into table
 *					FALSE - error while reading/processing measurements
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  30-Sep-1998 SFK		Created
 *	08-Aug-2002	SFK		Changed to DATE
===========================================================================*/
static BOOL ReadMeasurementsFromDbByTime()
{     
	int i, j;
	int status;
	struct db_event_rec dbChanEvent;
	BOOL bMatches;
	
	// allocate all storage will need for the table
	if (AllocateTableDialogBox() == FALSE) return(FALSE);

	// find first event for first station in the table before enter table
	bMatches = FALSE;
	status = ScanChannelEvents(glldStart, FIRST, &dbChanEvent);
	while ((status == TRUE) && (bMatches == FALSE)) {
		for (j=0; j<glliNumStas; j++) {			
			if (pgllbStaSelect[j] == TRUE) { // consider only events from stations selected
				if (EvaluateEvent(j, &dbChanEvent)){
					bMatches = TRUE;
					break;
				}
			}
		}
		if (!bMatches) status = ScanChannelEvents(glldStart, NEXT, &dbChanEvent);
	}
	if (bMatches != TRUE) return(FALSE); // no events for any of the stations

	for (i=0; i<glliNumMatchingEvents; i++) {
		if (bMatches) status = PutThisEventInTableRow(&dbChanEvent, &i, glliNumMatchingEvents);

		// Find next event
		bMatches = FALSE;
		do {
			status = ScanChannelEvents(glldStart, NEXT, &dbChanEvent);	// read next event in db
			if (status == TRUE) {	// does this event match any of the stations selected
				for (j=0; j<glliNumStas; j++) {			
					if (pgllbStaSelect[j] == TRUE) { // consider only events from stations selected
						if (EvaluateEvent(j, &dbChanEvent)) bMatches = TRUE;
					}
				}
			}
		} while ((status == TRUE) && (bMatches == FALSE));
		if ((status != TRUE) && ((i+1) < glliNumMatchingEvents)) return(FALSE); // problem when looking for the events.
	}	
	return (TRUE);
}                                                      


/*===========================================================================
 *
 *  Name	 :  	ReadMeasurementsFromDbByStation
 *
 *  Purpose	 :		Read all the matching measurements from the database.
 *					Put them in the table sorted by station
 *
 *  Return value :  TRUE - all measurements read and entered into table
 *					FALSE - error while reading/processing measurements
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  30-Sep-1998 SFK		Created
 *	08-Aug-2002	SFK		Changed to DATE
 *	14-Dec-2004	SFK		Added setting the gllnaszTypeList parameter when know station type
===========================================================================*/
static BOOL ReadMeasurementsFromDbByStation()
{     
	int j;
	int status;
	int iTableIndex;
	struct db_event_rec dbChanEvent;
	//short sSta;
	struct IDStruct StaID;
	BOOL bMatches;
	short sDum;

	// allocate all storage will need for the table
	if (AllocateTableDialogBox() == FALSE) return(FALSE);

	// find first event for first station in the table before enter table
	bMatches = FALSE;
	
	iTableIndex = 0;
	for (j=0; j<glliNumStas; j++) {			
		if (pgllbStaSelect[j] == TRUE) { // consider only events from stations selected
			// look for the first event from the station
			if (!pglFacCfg->GetStationIDForPicklistChoice(j, &StaID)) return(FALSE);
			StaID.sChanID = -1;		// 03-Jan-2006 SFK  -1 signals any channel
			GetEventTypePicklist(StaID, EXPORT_LIST, &gllnaszTypeList, &sDum);
			status = ScanChannelEvents(glldStart, FIRST, &dbChanEvent,StaID);
			if (status != TRUE) continue; // no events for this station
			if (dbChanEvent.d_event_beg_time_key <= glldEnd) {
				// look for the rest of the events with this station
				do {
					bMatches = EvaluateEvent(j, &dbChanEvent);	// right type of event??
					if (bMatches) 
						status = PutThisEventInTableRow(&dbChanEvent, &iTableIndex, glliNumMatchingEvents);
					if (status != TRUE) return (FALSE);
					status = ScanChannelEvents(glldStart, NEXT, &dbChanEvent,StaID ); // get next event for station
				} while ((status != uiEVENT_NOT_IN_DB_INFO) && (dbChanEvent.d_event_beg_time_key <= glldEnd)) ; // finished with this stations
			}

		}
	}
	if (iTableIndex == 0) return(FALSE); // no events for any of the stations
	return (TRUE);
}                                                      


/*===========================================================================
 *
 *  Name	 :  	AccessIdFile
 *
 *  Purpose	 :		Based on the specified action either
 *					READ - read all IDs from the file and sort the alphbetically
 *					WRITE - write a single ID to the end of the file
 *					in the lists for a table entry
 *
 *  Return value :  TRUE - event entered in row
 *					FALSE - invalid row requested
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  03-Oct-1998 SFK		Created
 ===========================================================================*/
static BOOL AccessIdFile(int iAction, char **pIDs[], short *psNumIDs)
{
	FILE *stream = NULL;
	size_t reserved_memory;
	short sNum, sNumIDs;
	int i;
	char line[100] ;


	if ((iAction != READ) || (iAction != WRITE)) return (FALSE);

	switch (iAction){
	case READ:
		if( (stream = fopen( "OPID.DB", "r+t" )) != NULL ) {
			if( fgets( line, 100, stream ) == NULL) {				// read the number of columns and toss
    			RadReviewMsg(uiFILE_READ_ERR, "OPID.DB");
    			return(FALSE);
			}

			// read the number of IDs and convert to a number
			if( fgets( line, 100, stream ) == NULL) {
    			RadReviewMsg(uiFILE_READ_ERR, "OPID.DB");
    			return(FALSE);
			}
			sNumIDs = atoi(line);

			// allocate storage for IDs to be read
			sNum = gen_allocate_str_array((ID_LEN+1), sNumIDs + 3,	reserved_memory, pIDs);
			if (sNum != *psNumIDs) {
		    	if (*pIDs) gen_deallocate_str_array (*pIDs);
				*pIDs = NULL;
    			RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
    			return(FALSE);
			}

			// read the IDs from the file and store in the array
			for (i=0; i<sNumIDs; i++) {
				if( fgets( line, 100, stream ) == NULL) {
    				RadReviewMsg(uiFILE_READ_ERR, "OPID.DB");
    				return(FALSE);
				}
				MyStrnCpy((*pIDs)[i], line, ID_LEN+1);
			}
		}
		else {	// no files exits
			sNumIDs = 0;
			sNum = gen_allocate_str_array((ID_LEN+1), sNumIDs + 3,	reserved_memory, pIDs);
			if (sNum != *psNumIDs) {
		    	if (*pIDs) gen_deallocate_str_array (*pIDs);
				*pIDs = NULL;
    			RadReviewMsg(uiMEMORY_ALLOCATION_ERR);
    			return(FALSE);
			}
			i=0;
		}
		MyStrnCpy((*pIDs)[i], ENTER_ID, ID_LEN+1);
		MyStrnCpy((*pIDs)[i+1], NO_ID, ID_LEN+1);
		MyStrnCpy((*pIDs)[i+2], NULL, ID_LEN+1);
		break;


   
	case WRITE:
		break;
	// open the file if this is the first write	
	}
	return(TRUE);
}

/*===========================================================================
 *
 *  Name	 :  	SelectAll
 *					DeselectAll
 *					ProcessIdFieldEntry
 *					LinkToGraph
 *
 *  Purpose	 :		Routines to service pushbuttons in the dialog box
 *					SelectAll - Mark all measurement selects to TRUE 
 *					DeselectAll - Mark all measurement selects to FALSE
 *					ProcessTableEdit - Action to take when ID field changed
 *					LinkToGraph - 
 *
 *  Return value :  TRUE - all measurements read and entered into table
 *					FALSE - error while reading/processing measurements
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  30-Sep-1998 SFK		Created
===========================================================================*/
static void SelectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGUI_Table *pTable;
    int i;
    
	pDlg->GetTable(IDC_EXPORT_MEASUREMENTS_TABLE, &pTable);
	if (pTable != NULL) {
	    for (i=0; i<glliNumMatchingEvents; i++) {
	    	pgllbSelectTableList[i] = TRUE;
	    }	
        pTable->UpdateCol(glluiCheckBoxCol);
    }
}        


static void DeselectAll(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGUI_Table *pTable;
	int i;
		
	pDlg->GetTable(IDC_EXPORT_MEASUREMENTS_TABLE, &pTable);
	if (pTable != NULL) {
	    for (i=0; i<glliNumMatchingEvents; i++) {
	    	pgllbSelectTableList[i] = FALSE;
	    }	
        pTable->UpdateCol(glluiCheckBoxCol);
    }
}        

//	Keep track of whether the user actually changes either the ID or the type in the table.
//	This is used to decide whether to ask the user to save the current values when the graph
//  and measurements table are linked.
static BOOL ProcessTableEdit(CGUI_Dlg *pDlg, CGUI_Table *pTable, UINT uiRow, UINT uiCol)
{
	CGUI_Table *pTbl;
	CGStr szTmp;
	DATE dDisplayStartTime;
	DATE dViewPeriodInDATE;
	DATE dBegTime;
	//CMyDateTime Date;

	// Always check to see if have changed the ID
	if ((gll_pDlg != NULL) && (uiCol == glluiIDCol)){
		gll_pDlg->RetrieveAllControlValues();	/* get dialog values */
		gll_pDlg->GetTable(IDC_EXPORT_MEASUREMENTS_TABLE, &pTbl);
		if (pTbl != NULL) {
			pTbl->RetrieveAllCellValues(); 	/* get table values */
			pTbl->TxtGetValue(uiRow, uiCol, &szTmp);
			strcpy(gllszID[uiRow], szTmp.Get_sz()); // always copy the text to my "master list"
		}
	}

	struct IDStruct TempID;
	short sPicklistChoice;
	// Only write the event specified by the row to the db if the real time link is enabled
	if (gllbRealTimeUpdates == TRUE) {
		if (gll_pDlg != NULL) {
			gll_pDlg->RetrieveAllControlValues();	/* get dialog values */
			gll_pDlg->GetTable(IDC_EXPORT_MEASUREMENTS_TABLE, &pTbl);
			if (pTbl != NULL) {
				pTbl->RetrieveAllCellValues(); 	/* get table values */
				pTbl->TxtGetValue(uiRow, uiCol, &szTmp);
				strcpy(gllszID[uiRow], szTmp.Get_sz());
			}
			UpdateSingleEventInDb(uiRow);			// write the modified event to the db
			GraphGetViewPeriodParameters(&dDisplayStartTime, &dViewPeriodInDATE);
			if ((pglldOrgBeg[uiRow] < dDisplayStartTime) || (pglldOrgBeg[uiRow] > (dDisplayStartTime + dViewPeriodInDATE))) {
				dBegTime = pglldOrgBeg[uiRow] - dViewPeriodInDATE/2.0;   	
				GraphSetDisplayDate(dBegTime, dViewPeriodInDATE);
			}

			TempID.sStaID = pgllsSta[uiRow];
			TempID.sStaType = pgllsStaType[uiRow];
			TempID.sChanID = pgllsChan[uiRow];
			if (!pglFacCfg->GetPicklistChoiceForChannelID(TempID, &sPicklistChoice)) return(FALSE);
			GraphSetCursorTime(pglldOrgBeg[uiRow], sPicklistChoice);	// cursor to event beginning
			UpdateGraphData(TRUE);		// have the graph display the latest data
			GraphSetCursorTime(pglldOrgBeg[uiRow], sPicklistChoice);	// cursor to event beginning
		}
	}
	return(TRUE);
}


#define NUM_IDS 4
static char *gllnaszDummyIds[NUM_IDS] = {"A12345", "D56789", "C24680", NULL};               
//  When the graph and the export table are "logically linked", this function is called
//  each time the graph writes a new event to the database.  All measurements are again
//	read from the database and a "new updated" table is displayed.
//	08-Aug-2002	SFK		Changed to DATE
static void RereadDbAndUpdateTable()
{
	CGUI_Table *pTbl;
	CGFmtStr msg;
    BOOL bOkay = FALSE;
    
	gll_pDlg->GetTable(IDC_EXPORT_MEASUREMENTS_TABLE, &pTbl);
	if (pTbl != NULL) {
		gll_pDlg->SetVisible(FALSE);  
		pTbl->Clear();

		// count how many events match conditions specified by the previous dialog box
		// this may now be different because the user might have added an event on the graph
		CountMatchingEvents(&glliNumMatchingEvents);
		if (glliNumMatchingEvents == 0) return;

		//  Read the measurements that will be displayed in the table
		//	and make up the new table lists
		bOkay = ReadMeasurementsFromDbByStation();
		if (!bOkay) {	// some error during the reading of the measurements, clear table
			gll_pDlg->GetTable(IDC_EXPORT_MEASUREMENTS_TABLE, &pTbl);
			if (pTbl != NULL) pTbl->Clear();
			glliNumMatchingEvents = 0;
			return;
		}	
		
		glluiCheckBoxCol = pTbl->AddColCheckBox("Export", glliNumMatchingEvents, pgllbSelectTableList);
		glluiIDCol = pTbl->AddColTxt("Item ID", glliNumMatchingEvents, pgllszIDTableList, ID_LEN+1, "%s", ProcessTableEdit);
		glluiTypePickListCol = pTbl->AddColPickList("Type", glliNumMatchingEvents, pglluiTypeTableList, gllnaszTypeList, ProcessTableEdit);			
		glluiStaCol = pTbl->AddColTxt("Station", glliNumMatchingEvents, pgllszStaNameTableList, gllsMaxStaNameLen);
		glluiDateCol = pTbl->AddColTxt("Date", glliNumMatchingEvents, pgllDateTableList, DT_LEN+1);			
		glluiStartTimeCol = pTbl->AddColTxt("Start", glliNumMatchingEvents, pgllStartTimeTableList, DT_LEN+1);			
		glluiEndTimeCol = pTbl->AddColTxt("End", glliNumMatchingEvents, pgllEndTimeTableList, DT_LEN+1);	
		pTbl->SetColWriteEnable(glluiDateCol, FALSE);
		pTbl->SetColWriteEnable(glluiStartTimeCol, FALSE);
		pTbl->SetColWriteEnable(glluiEndTimeCol, FALSE);
		pTbl->SetColWriteEnable(glluiStaCol, FALSE);

		pTbl->DisplayNew();
		gll_pDlg->SetVisible(TRUE);  
	}
}	


// Called from LiveTime GraphLink Button
//	Sets up the realtime link between the table and the graph.  First asks question whether really
//	want to do the link.  If yes, retrieves all the values and updates the database.  Then displays
//	the graph and registers a function that will update the table each time the graph changes an event.
static void LinkToGraph(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	CGFmtStr msg;
	CGUI_Table *pTbl;

	// only take this action once
	if (gllbLinked == FALSE) {
		msg.Printf("You have selected to link the graph and this table together.");
		msg.PrintfAppend("\nDuring the link, all changes are immediately stored in the database.");
		msg.PrintfAppend("\nDo you want to proceed with the link?");
		if (GUI_MsgBox(msg, GUI_ICON_QUESTION, GUI_YESNO) != GUI_YES) return;
		gllbLinked = TRUE;
		if (gll_pDlg != NULL) {
			gll_pDlg->RetrieveAllControlValues();	/* get dialog values */
			gll_pDlg->GetTable(IDC_EXPORT_MEASUREMENTS_TABLE, &pTbl);
			if (pTbl != NULL) pTbl->RetrieveAllCellValues(); 	/* get table values */
			UpdateAllEventsInDb();		// write all events that might have been changed in the table to the db
			UpdateGraphData(TRUE);		// have the graph display the latest data
			RegisterTableUpdateFunction(RereadDbAndUpdateTable); // must do an update after any table action now
			gllbRealTimeUpdates = TRUE;
			pDlg->SetText(IDOK, "Export");
		}
	}
	UpdateGraphData(TRUE);
}





/*===========================================================================
 *
 *  Name	 :  ExportTableDialogBox
 *
 *  Purpose	 :  Display the dialog box with the table of all the measurements.
 *				Allow the user to select whether to export a measurement or not,
 *				to enter an ID and to select a different measurement type??
 *
 *  Return value :  TRUE/FALSE
 *
 *  Implicit Input : 
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK		Created
 *	08-Aug-2002	SFK		Changed to DATE
 *
===========================================================================*/
static BOOL ExportTableDialogBox(BOOL bAutomated)
{                                 
	// these parameters remain unchanged for each execution of the menu option                                                    
	static BOOL bFirst = TRUE;
    	
    GUI_ACTION DlgReturn;                                     
    CGUI_Table *pTbl; 
	char  szSrchStart[DT_LEN+1], szSrchEnd[DT_LEN+1], szDum[DT_LEN+1];              	
	int i;    

    short sNumMeasToExport;
	//CMyDateTime Date;

	gllbLinked = FALSE;
	gllbRealTimeUpdates = FALSE;
	gllsFacNum = glsFacNum;
	// verify the facility is defined
	char szFacName[DB_NAME_LEN+1];
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) return(FALSE);
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) return (FALSE);

	pglFacCfg->GetFacilityLongName(glsFacNum, szFacName, DB_NAME_LEN);

	// count how many events match conditions specified by the previous dialog box
	CountMatchingEvents(&glliNumMatchingEvents);
	if (glliNumMatchingEvents == 0) return(FALSE);
		    
   /* ------------------------------------------------------------------
    *	Define the dialog box
    * ----------------------------------------------------------------*/
    CGUI_Dlg Dlg(IDD_TOOLS_EXPORT_TABLE, NULL, DLG_POSN_NO_SAVE);
	if (Dlg.IsValid()) {
		gll_pDlg = &Dlg;
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
		Dlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		Dlg.DefinePushBtn(IDOK, GUI_OK);                   
		Dlg.DefinePushBtn(IDC_EXPORT_SELECT, GUI_NONE, SelectAll);          
		Dlg.DefinePushBtn(IDC_EXPORT_DESELECT, GUI_NONE, DeselectAll); 
		Dlg.DefinePushBtn(IDC_UPDATE_GRAPH, GUI_NONE, LinkToGraph);  		
		Dlg.SetVisible(IDC_UPDATE_GRAPH, TRUE);  // show or not based on .ini parameter
		
	   /* ------------------------------------------------------------------
	    *	Fill in facility name
	    * ----------------------------------------------------------------*/
		Dlg.DefineFldStaticTxt(IDC_EXPORT_FAC_NAME);
		Dlg.FldStaticTxtSetValue(IDC_EXPORT_FAC_NAME, szFacName);
		
	   /* ------------------------------------------------------------------
	    *	Display search start, end, interval using info gotten in
		*	prep. for showing the previous screen
	    * ----------------------------------------------------------------*/
		glMyDateTime.DATETimestampToDateTimeStrs(glldStart, szSrchStart, szDum);
		glMyDateTime.DATETimestampToDateTimeStrs(glldEnd, szSrchEnd, szDum);
		Dlg.DefineFldStaticTxt(IDC_EXPORT_CURR_START);
		Dlg.FldStaticTxtSetValue(IDC_EXPORT_CURR_START, szSrchStart);
		Dlg.DefineFldStaticTxt(IDC_EXPORT_CURR_END);
		Dlg.FldStaticTxtSetValue(IDC_EXPORT_CURR_END, szSrchEnd);

		//  Read the measurements that will be displayed in the table
		//TempID.sChanID = ANY_STATION;
		//TempID.sStaID = ANY_STATION;
		//TempID.sStaType = ANY_TYPE;
		//GetEventTypePicklist(TempID, EXPORT_LIST, &gllnaszTypeList, &dum);
		ReadMeasurementsFromDbByStation();


		// Define the table and its columns
		gllbEditsInMeasurementTable = FALSE;
		Dlg.DefineTable(IDC_EXPORT_MEASUREMENTS_TABLE, &pTbl);	
		if (pTbl != NULL) {
			pTbl->SetColWidths(CGUI_TABLE_COL_WIDTH_SCALED);
			glluiCheckBoxCol = pTbl->AddColCheckBox("Export", glliNumMatchingEvents, pgllbSelectTableList);
			glluiIDCol = pTbl->AddColTxt("Item ID", glliNumMatchingEvents, pgllszIDTableList, ID_LEN+1, "%s", ProcessTableEdit);
			glluiTypePickListCol = pTbl->AddColPickList("Type", glliNumMatchingEvents, pglluiTypeTableList, gllnaszTypeList, ProcessTableEdit);			
			glluiStaCol = pTbl->AddColTxt("Station", glliNumMatchingEvents, pgllszStaNameTableList, gllsMaxStaNameLen);
			glluiDateCol = pTbl->AddColTxt("Date", glliNumMatchingEvents, pgllDateTableList, DT_LEN+1);			
			glluiStartTimeCol = pTbl->AddColTxt("Start", glliNumMatchingEvents, pgllStartTimeTableList, DT_LEN+1);			
			glluiEndTimeCol = pTbl->AddColTxt("End", glliNumMatchingEvents, pgllEndTimeTableList, DT_LEN+1);	
			pTbl->SetColWriteEnable(glluiDateCol, FALSE);
			pTbl->SetColWriteEnable(glluiStartTimeCol, FALSE);
			pTbl->SetColWriteEnable(glluiEndTimeCol, FALSE);
			pTbl->SetColWriteEnable(glluiStaCol, FALSE);
		}
			
		Dlg.SetFocus(IDC_EXPORT_MEASUREMENTS_TABLE);
		// keep dialog box on top
		::SetWindowPos(Dlg.GetWndHandle(), HWND_TOP, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
		
		DlgReturn = Dlg.Go();
		gll_pDlg = NULL;
		RegisterTableUpdateFunction(NULL);
			    
	   /* ------------------------------------------------------------------
	    *	Returned from dialog box, get the values now.
	    * ----------------------------------------------------------------*/
	    if (DlgReturn == GUI_OK) {  
			Dlg.RetrieveAllControlValues();	/* get dialog values */
			pTbl->RetrieveAllCellValues(); 	/* get table values */

			UpdateAllEventsInDb();				/* if any events were edited, write results to db */

		   /* ------------------------------------------------------------------
		    *	Check that they selected at least 1 measurement
		    * ----------------------------------------------------------------*/
		    sNumMeasToExport = 0;
			for (i=0; i<glliNumMatchingEvents; i++) {
				if (pgllbSelectTableList[i] == TRUE) {
					sNumMeasToExport++;
				}	
			}		
		    if (sNumMeasToExport == 0) {
		        RadReviewMsg(uiNO_MEAS_SELECTED_WARN);
		    	return(FALSE);
		    }	
	    	return(TRUE);
		}
	}	
	return(FALSE);
}
  

                            
/////////////////////////////////////////////////////////////////////////////////////
//	CloseTextWindow
//	Allows the user to close the window showing the results
/////////////////////////////////////////////////////////////////////////////////////
static BOOL CloseTextWindow(CGUI_TextWin *pWinArg)
{
	BOOL bCanClose = TRUE;
	pWin = NULL;
	return(bCanClose);
}               


/*===========================================================================
 *
 *  Name	 :  DoIt
 *
 *  Purpose	 :  Find the matching measurements and write to NCC, IR or XML file.
 *
 *  Return value :  TRUE/FALSE
 *
 *  Implicit Input :
 *  Implicit Input/Output:
 *  Implicit Output:
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  			SFK		Created
 *	08-Aug-2002	SFK		Changed to DATE
 *	13-Dec-2004	SFK		For SR data write individual files for INCC and possibly an IR file
 *						For GRAND data, call the routine that writes the XML file for FDMS
 *	03-Jan-2005	SFK		Changed to have separate check boxes for the different file writes
 *  21-Jun-2005 SFK		Change to use the "actual" time rather than coverting strings back to time since
 *						this doesn't work correctly with our floating point DATE numbers.
 *
===========================================================================*/
static BOOL DoIt()
{                                 
        
	struct db_event_rec dbChanEvent, dbChanEvent1;
	int status;
	int i;
	char szFileName[MAX_LINE_LEN+1];
//	char szStationName[12];
	unsigned long ulNumSelected, ulNumPrinted;
	unsigned short usDirection;	GUI_ACTION response;
	BOOL bNccWriteOkay = TRUE;
	//CMyDateTime Date;
	BOOL bWriteToXML = FALSE;

   /* ------------------------------------------------------------------
    *	Open up the text window where the events will be displayed
    * ----------------------------------------------------------------*/
	if (pWin == NULL) {                
		pWin = new CGUI_TextWin("Data Exported");
		if (pWin != NULL) {
			pWin->ShowStatusBar(TRUE);
		}	
	}   
	else {
		pWin->Clear();
	}
	if (pWin != NULL) pWin->SetFocus();

	// print the facility specific header
	if (!PrintFacilityHeader(pWin, gllsFacNum, glldStart, glldEnd)) return(FALSE);

	if (pWin) pWin->Printf("\n\nThe following events were exported:");
	if (pWin) pWin->Printf("\nItem ID        Event Type             Station     Dir      Date       Start      End        Filename");
	if (pWin) pWin->Printf("\n                                                                      Time       Time");

	// determine how many are selected, need this for the file to write to IR
	ulNumSelected = 0;
	for (i=0; i<glliNumMatchingEvents; i++) {
		if (pgllbSelectTableList[i] == TRUE) ulNumSelected++;
	}

	ulNumPrinted = 0;
	short sAction;
	BOOL bFirstEvent = TRUE;
	for (i=0; i<glliNumMatchingEvents; i++) {
		if (pgllbSelectTableList[i] == TRUE) {
			// from information in the table figure out the beg and end time, handle case of end time in next day
			// 21-Jun-2005 SFK Change to use the "actual" time rather than coverting strings back to time since
			//					this doesn't work correctly with our floating point DATE numbers.
			dbChanEvent.d_event_beg_time_key = pglldOrgBeg[i];
			dbChanEvent.d_event_end_time = pglldOrgEnd[i];

			// Change if put measurements on different channels.
			dbChanEvent.us_event_chan = pgllsChan[i];
			dbChanEvent.us_event_sta = pgllsSta[i];
			dbChanEvent.us_event_sta_type = pgllsStaType[i];

			status = ReadEventRec(&dbChanEvent, &dbChanEvent1);
			if (status != TRUE) continue;		// couldn't find this event to export
			MyStrnCpyAndPad(dbChanEvent1.sz_event_id, pgllszIDTableList[i], ID_LEN+1, ' ' ); // update the id in the event to send

			// 1-3-2005 SFK	Set event bits for SR type measurements
			if ((dbChanEvent.us_event_sta_type == JSR_TYPE) || (dbChanEvent.us_event_sta_type == JSR1_TYPE) 
				|| (dbChanEvent.us_event_sta_type == ISR_TYPE)) {
				switch (pglluiTypeTableList[i]) {		// update the measurement type
				case EXPORT_ASSAY_PL_CHC:
					dbChanEvent1.us_event_type = MEASUREMENT_EVENT;
					dbChanEvent1.us_event_type1 = ASSAY_MEAS;
					break;

				case EXPORT_CF_PL_CHC:
					dbChanEvent1.us_event_type = MEASUREMENT_EVENT;
					dbChanEvent1.us_event_type1 = CF_NORM_MEAS;
					break;

				case EXPORT_NORM_BKG_PL_CHC:
					dbChanEvent1.us_event_type = BACKGROUND_EVENT;
					dbChanEvent1.us_event_type1 = NORMAL_BKG_MEAS;
					break;

				case EXPORT_TOTALS_ASSAY_PL_CHC:
					dbChanEvent1.us_event_type = MEASUREMENT_EVENT;
					dbChanEvent1.us_event_type1 = TOTALS_MEAS;
					break;

				case EXPORT_EMPTY_ASSAY_PL_CHC:
					dbChanEvent1.us_event_type = MEASUREMENT_EVENT;
					dbChanEvent1.us_event_type1 = EMPTY_MEAS;
					break;
				}
			}

			// 01-Jan-2005 SFK Set event bits for GRAND type measurements
			if (dbChanEvent.us_event_sta_type == GRAND_TYPE) {
				switch (pglluiTypeTableList[i]) {		// update the measurement type
				case GRAND_CHAN_PL_CHC:
					dbChanEvent1.us_event_type = CHANNEL_EVENT;
					dbChanEvent1.us_event_type1 = UNKNOWN_MEAS;
					break;

				case GRAND_BKGD_PL_CHC:
					dbChanEvent1.us_event_type = BACKGROUND_EVENT;
					dbChanEvent1.us_event_type1 = NORMAL_BKG_MEAS;
					break;
				}
			}

			// 04-Jan-2005 SFK write the IR file if indicated by checkbox
			if (gllbIRFile == TRUE) {
				PrintMeasEventToTextDBFile(&dbChanEvent1, ulNumPrinted, ulNumSelected, &usDirection);
				ulNumPrinted++;
			}
			else {
				usDirection = NO_DIRECTION;	// if error on write, use no direction 
			}

			// 03-Jan-2005 SFK write the NCC file if indicated by checkbox
			if (gllbNCCFile == TRUE) {
				status = WriteMeasToNCCFile(&dbChanEvent1, szFileName);
				// Print results of export to text window
				//MyStrnCpyAndPad(szStationName, pgllszStaNameTableList[i], 12, ' ' ); // update the id in the event to send
				pWin->Printf("\n%s   %s   %s   %s   %s   %s   %s   ", 
					dbChanEvent1.sz_event_id, gllnaszTypeList[pglluiTypeTableList[i]], pgllszStaNameTableList[i], glnaszDirectionList[usDirection], pgllDateTableList[i], pgllStartTimeTableList[i], pgllEndTimeTableList[i]);
				if (status) {
					pWin->Printf("%s", szFileName);
				}
				else {
					pWin->Printf("File could not be exported to NCC");
					bNccWriteOkay = FALSE;
					response = GUI_MsgBox("There was a problem exporting the file to INCC.  Do you want to abort the export process?", GUI_ICON_QUESTION, GUI_YESNO);
					if (response == GUI_YES) break;
				}
			}


			if (gllbXMLFile == TRUE) {
				// Write the XML file
				if (bFirstEvent) 
				{
					sAction = FIRST_EVENT;
					bFirstEvent = FALSE;
				}
				else 
					sAction = NEXT_EVENT;

				WriteEventSummaryToXmlFile(&dbChanEvent1, sAction, szFileName);
				// print information written to XML file
				pWin->Printf("\n%s   %s   %s   %s   %s   %s   %s   ", 
					//dbChanEvent1.sz_event_id, gllnaszTypeList[pglluiTypeTableList[i]], szStationName, glnaszDirectionList[usDirection], pgllDateTableList[i], pgllStartTimeTableList[i], pgllEndTimeTableList[i]);
					dbChanEvent1.sz_event_id, gllnaszTypeList[pglluiTypeTableList[i]], pgllszStaNameTableList[i], glnaszDirectionList[usDirection], pgllDateTableList[i], pgllStartTimeTableList[i], pgllEndTimeTableList[i]);
				pWin->Printf("%s", szFileName);
			}
		}
	}
	if (pWin != NULL) pWin->EnableUserClose(CloseTextWindow);    

	// finished writing to the text database file so close it and delete the object
	if (gllpWriteDB != NULL) {
		gllpWriteDB->Database_Close();
		delete gllpWriteDB;
		gllpWriteDB = NULL;
	}
	if (gllbXMLFile) WriteEventSummaryToXmlFile(&dbChanEvent1, WRITE_EVENT_LIST_TO_XML, szFileName);	// actually write the XML file

	return(bNccWriteOkay);
}	      


   		                                 
/*===========================================================================
 *
 *  Name	 :  	ExportOptionsDialog
 *
 *  Purpose	 :		Display the dialog box that appears first off the main
 *					menu.  This allows the user to specify the time interval
 *					of which measurements to export, which station(s) measurements
 *					to export and and use checkboxes to indicate which types
 *					of measurements to export.
 *
 *  Return value :  None
 *
 *
 *  date    /	author	revision
 *  -----------------	--------
 *  29-Mar-1998 SFK		Created
 *  30-Sep-1998 SFK		Use checkboxes for measurement types 
 *	08-Aug-2002	SFK		Changed to DATE
 *	13-Dec-2004	SFK		Added export channel stuff
 *
===========================================================================*/
static BOOL ExportOptionsDialogBox(BOOL bAutomated)
{                                 
	static BOOL bFirst = TRUE;
	//static char szOldDbName[DRIVE_LEN+PATH_LEN+1] = {""};
	static DATE dLastSearchIntervalInDays = 0.0;
	CGFmtStr szInterval;                                   
	int status = TRUE;     
	short sNumStas;
	BOOL bOptionOk = FALSE;
	DATE dSearchIntervalInDays;
	int i;
	BOOL bExportAssay = FALSE;
	BOOL bExportCf = FALSE;
	BOOL bExportNormBkg = FALSE;
	BOOL bExportEmptyAssay = FALSE;
	BOOL bExportTotalsAssay = FALSE;
	BOOL bExportChannel = FALSE;
//	BOOL bWriteFile;
	
	GUI_ACTION DlgReturn;                                     
	CGFmtStr msg;  
	CGUI_Table *pTbl;    
	//CMyDateTime Date;

	// verify the facility is defined
	char szFacName[DB_NAME_LEN+1];
	if (!pglFacCfg->IsFacilityDefined(glsFacNum)) return(FALSE);
	// 3.0.3.16 Dec 1, 2004  Check that there are some stations in the currently defined facility
	if (!pglFacCfg->DoesFacilityHaveAnyData(glsFacNum)) return(FALSE);

	pglFacCfg->GetFacilityLongName(glsFacNum, szFacName, DB_NAME_LEN);

   /* ------------------------------------------------------------------
    *	Always find the first and last days in the database and set
	*	up the strings for display in the dialog box.
    * ----------------------------------------------------------------*/
	SetCurrentDrive();
	status = GetDatabaseIntervalParameters(gllszFirstDbDate, gllszLastDbDate, &gllulDbIntervalInDays, &dSearchIntervalInDays);
	if (status != TRUE) return(FALSE);
	if (dSearchIntervalInDays <= 0.0) {
		RadReviewMsg(uiDB_EMPTY_STATUS);
		return(FALSE);
	}

    if ((bFirst == TRUE) || (!SameDb(glszOldDbName)) || (dSearchIntervalInDays != dLastSearchIntervalInDays) || (gllsFacNum != glsFacNum)) {
		gllsFacNum = glsFacNum;
		ReadDefaultDbParameters();
		GetReviewPeriodParameters(&glldStart, &glldEnd, &gllulUserInterval);
		glluiUnits = 0;

		// remember the current conditions for future entries to this option
	    GetDbName(glszOldDbName);								
		dLastSearchIntervalInDays = dSearchIntervalInDays;	
		bFirst = FALSE;

	}		

   /* ------------------------------------------------------------------
    *	Create dialog box to display the main export dialog options information
    * ----------------------------------------------------------------*/
    CGUI_Dlg ExDlg(IDD_TOOLS_EXPORT_OPTION, NULL, DLG_POSN_NO_SAVE);
    if (ExDlg.IsValid()) {
	   /* ------------------------------------------------------------------
	    *	Define the buttons and actions in the dialog box
	    * ----------------------------------------------------------------*/
		ExDlg.DefinePushBtn(IDCANCEL, GUI_CANCEL);
		ExDlg.DefinePushBtn(IDOK, GUI_OK);
		ExDlg.DefinePushBtn(IDC_EXPORT_ALL_BUTTON, GUI_NONE, SelectAllSta);          
		ExDlg.DefinePushBtn(IDC_EXPORT_NONE_BUTTON, GUI_NONE, DeselectAllSta); 
		ExDlg.DefinePushBtn(IDUPDATE, GUI_NONE, UpdateDbParms);  		
				                                               
	   /* ------------------------------------------------------------------
	    *	Define the facility
	    * ----------------------------------------------------------------*/
		ExDlg.DefineFldStaticTxt(IDC_EXPORT_FACILITY);
		ExDlg.FldStaticTxtSetValue(IDC_EXPORT_FACILITY, szFacName);

	   /* ------------------------------------------------------------------
	    *	Display current starting ending database dates and interval
	    * ----------------------------------------------------------------*/
		ExDlg.DefineFldStaticTxt(IDC_EXPORT_CURR_DB_START);
		ExDlg.FldStaticTxtSetValue(IDC_EXPORT_CURR_DB_START, gllszFirstDbDate);
		ExDlg.DefineFldStaticTxt(IDC_EXPORT_CURR_DB_END);
		ExDlg.FldStaticTxtSetValue(IDC_EXPORT_CURR_DB_END, gllszLastDbDate);

		szInterval.Printf("%ld days", gllulDbIntervalInDays);  	
		ExDlg.DefineFldStaticTxt(IDC_EXPORT_CURR_DB_INTERVAL);
		ExDlg.FldStaticTxtSetValue(IDC_EXPORT_CURR_DB_INTERVAL, szInterval);
		
	   /* ------------------------------------------------------------------
	    *	Fill in default values for user selectable search start date and time plus interval
	    * ----------------------------------------------------------------*/
		//ConvertSecondsToStructs(&gllulStart, &gllGenDate, &gllGenTime);
		glMyDateTime.DATETimestampToGenStructs(glldStart, &gllGenDate, &gllGenTime);
		ExDlg.DefineFldDate(IDC_EXPORT_SRCH_STRTDATE, &gllGenDate);
		ExDlg.DefineFldTime(IDC_EXPORT_SRCH_STRTTIME, &gllGenTime);
		ExDlg.DefineFldNum(IDC_EXPORT_SRCH_INTERVAL, &gllulUserInterval, 1, 10000,"%ld");
		ExDlg.DefinePickList(IDC_EXPORT_INTRVL_UNITS_PL, glnaszIntervalUnitsPicklist, &glluiUnits);
	                             
			
	   /* ------------------------------------------------------------------
	    *	Create a table consisting of columns for selection and the station names.
	    *	Allow user to select multiple stations that data will be exported from. 
	    * ----------------------------------------------------------------*/
		ExDlg.DefineTable(IDC_EXPORT_PICKTABLE, &pTbl);	
		if (pTbl != NULL) {
			glluiCheckBoxCol = pTbl->AddColCheckBox("Select", glliNumStas, pgllbStaSelect);		
			gllusStaCol = pTbl->AddColTxt("Station", glliNumStas, gllaszStaList, gllsMaxStaNameLen+1);
			pTbl->SetColWriteEnable(gllusStaCol, FALSE);	// cannot edit the station names
		}	
			                             
	   /* ------------------------------------------------------------------
	    *	Fill in the checkboxes to indicate types of measurements the
		*	user wants to export
	    * ----------------------------------------------------------------*/
		// Explode gllusMeasType into individual components
		if ((gllusMeasType & EXPORT_ASSAY_BIT) == EXPORT_ASSAY_BIT) bExportAssay = TRUE;
		if ((gllusMeasType & EXPORT_CF_BIT) == EXPORT_CF_BIT) bExportCf = TRUE;
		if ((gllusMeasType & EXPORT_NORM_BKG_BIT) == EXPORT_NORM_BKG_BIT) bExportNormBkg = TRUE;
		if ((gllusMeasType & EXPORT_EMPTY_ASSAY_BIT) == EXPORT_EMPTY_ASSAY_BIT) bExportEmptyAssay = TRUE;
		if ((gllusMeasType & EXPORT_TOTALS_ASSAY_BIT) == EXPORT_TOTALS_ASSAY_BIT) bExportTotalsAssay = TRUE;
		if ((gllusMeasType & EXPORT_CHANNEL_BIT) == EXPORT_CHANNEL_BIT) bExportChannel = TRUE;
		ExDlg.DefineCheckBox(IDC_EXPORT_ASSAY, &bExportAssay);	
		ExDlg.DefineCheckBox(IDC_EXPORT_CF, &bExportCf);	
		ExDlg.DefineCheckBox(IDC_EXPORT_NORM_BKG, &bExportNormBkg);	
		ExDlg.DefineCheckBox(IDC_EXPORT_EMPTY_ASSAY, &bExportEmptyAssay);	
		ExDlg.DefineCheckBox(IDC_EXPORT_TOTALS_ASSAY, &bExportTotalsAssay);	
		ExDlg.DefineCheckBox(IDC_EXPORT_CHANNEL, &bExportChannel);	

	   /* ------------------------------------------------------------------
	    *	Fill in information about the tolerance to use in looking for matching ids
	    * ----------------------------------------------------------------*/
		ExDlg.DefineFldNum(IDC_EXPORT_TOLERANCE, &gllulTolerance, 0, 86400,"%ld");


		ExDlg.DefineCheckBox(IDC_EXPORT_SRCH_FILE, &gllbIRFile);
		ExDlg.DefineCheckBox(IDC_EXPORT_SRCH_FILE1, &gllbXMLFile);
		ExDlg.DefineCheckBox(IDC_EXPORT_SRCH_FILE2, &gllbNCCFile);


		ExDlg.SetFocus(IDC_EXPORT_SRCH_INTERVAL);
		if (bAutomated) ExDlg.SetFocus(IDOK);
			
	    DlgReturn = ExDlg.Go();
	    if (DlgReturn == GUI_OK) {  
	    	ExDlg.RetrieveAllControlValues();					// get all values from dialog box
	    	if (pTbl != NULL) pTbl->RetrieveAllCellValues();	// retrieve the table values

			//pRAD_Init->Set_WriteMeasurementResults(bWriteFile);

			// Implode the individual components back into gllusMeasType
			gllusMeasType = 0;
			if (bExportAssay)		gllusMeasType = gllusMeasType | EXPORT_ASSAY_BIT;
			if (bExportCf)			gllusMeasType = gllusMeasType | EXPORT_CF_BIT; 
			if (bExportNormBkg)		gllusMeasType = gllusMeasType | EXPORT_NORM_BKG_BIT;
			if (bExportEmptyAssay)	gllusMeasType = gllusMeasType | EXPORT_EMPTY_ASSAY_BIT;
			if (bExportTotalsAssay) gllusMeasType = gllusMeasType | EXPORT_TOTALS_ASSAY_BIT;
			if (bExportChannel)		gllusMeasType = gllusMeasType | EXPORT_CHANNEL_BIT;

			//	Check that they selected at least 1 station
		    sNumStas = 0;
		    i = 0;
			while ((sNumStas == 0) && (i<glliNumStas)) {
				if (pgllbStaSelect[i] == TRUE) {
					sNumStas++;
				}
				i++;
			}	
		    if (sNumStas == 0) {
		    	RadReviewMsg(uiNO_STAS_SELECTED_WARN);
		    	return(FALSE);
		    }	

			// convert back from gen structs to program "time base" 
			status = ConvertStructsToSearchIntervalParameters(&gllGenDate, &gllGenTime, glluiUnits, gllulUserInterval, &glldStart, &glldEnd);
			if (status != TRUE) {	// no data for any station in the interval
				RadReviewMsg(uiNO_DATA_IN_INTERVAL_WARN, glldStart, glldEnd, "any station");
				return(FALSE);
			}

			// Check that there is at least one event for one of the stations
			CountMatchingEvents(&glliNumMatchingEvents);
			if (glliNumMatchingEvents == 0) {
		    	RadReviewMsg(uiNO_MEAS_TYPE_IN_INTERVAL_WARN, glldStart, glldEnd);
				return(FALSE);
			}

			bOptionOk = TRUE;	// have valid data to try to export
	    }	      
	}    
	return(bOptionOk);
}   		                                 


