///////////////////////////////////////////////////////////////////////////
//	Copyright, 2001, The Regents of the University of California.  This
//	software was produced under U.S. Government contract (W-7405-ENG-36) at
//	the Los Alamos National Laboratory by the Safeguards Science and
//	Technology Group (NIS-5)
//
//    File Name : Plt_evnt.cpp     
//Primary Author: Tom Marks

///////////////////////////////////////////////////////////////////////////
// $History: $
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	This file contains the following functions:
//
///////////////////////////////////////////////////////////////////////////
#include <gui_lib.h>
#include <gen.h>
#include "plt_pane.h"
#include "plt_db.h"
#include "plt_evnt.h"
#include "rad.h"
#include "EventTypeLists.h"
#include "FacilityConfig.h"
#include "MyDateTime.h"

extern CMyDateTime glMyDateTime;

// ============================================================================
// =================== Show / Edit Event Details Dialog Fns ===================
// ============================================================================

/*

Create New Event:
	set bEnbBoxNew = TRUE;
	set Mode to "creating new event"
	disable Prev, Next, Edit and Delete Btns.
	disable "Create new event"
	re-label "close" btn to "Cancel new event"
	when new box has been drawn:
		check that it does not overlap other regions.
		automatically switch to "edit event" mode.

Edit event:
	set bEnbResize and bEnbMove to TRUE
	draw box around event.
	set mode to "editing event"
	disable Prev, Next, Delete and Create btns.
	enable edit fields.
	re-label "edit event" to "store edited event"
	when "store" is hit check for event overlap.
	re-label "close" to "cancel edit"

*/


COLORREF CEditEvents::mEmphasizedColor = GUI_MAGENTA;
float CEditEvents::mfEmphasizedLineThicknessPts = 3.0;

		
CEditEvents::CEditEvents(CPlotDlg *pParentPlotDlg, CPlotPane *pPlotPane, CPlotDB *pPlotDB)
{
	mpParentPlotDlg = pParentPlotDlg;
	mpPlotPane = pPlotPane;
	mpPlotDB = pPlotDB;
	mpEditEventDlg = NULL;
	meMode = EEM_NONE;
	mpPlObjBox = NULL;
	mbEmphasizeActive = FALSE;

	CreateDlg();
}


CEditEvents::~CEditEvents()
{
	if (mpEditEventDlg != NULL) {
		mpEditEventDlg->Close(GUI_CANCEL);
	}
}



void CEditEvents::CreateDlg()
{
	const UINT uiEVENT_ID_DIM = 13;	// inferred from db_event_rec sz_event_id field

	static struct GEN_TIME_STRUCT StartTime;
	static struct GEN_TIME_STRUCT EndTime;
	static CGStr EventID("ID String");
	static CGStr EventType("Event Type");
	static CGStr EventChannel("Event Channel");
	BOOL bAlwaysOnTop = TRUE;
	UINT uiPickIndex;
	char *naszList[] = {NULL}; 

	muiPrevUpdateDlgCursorTraceIndex = 0;
	mdPrevUpdateDlgCursorLowValueRelSecs = 0.0;
	mdPrevUpdateDlgCursorHighValueRelSecs = 0.0;

	if (mpEditEventDlg == NULL) {
		mpEditEventDlg = new CGUI_Dlg(IDD_GRAPH_EDIT_EVENTS, NULL, DLG_POSN_SAVE_ABS);
		if (mpEditEventDlg != NULL) {
			mpEditEventDlg->DefineCheckBox(IDC_ALWAYS_ON_TOP, &bAlwaysOnTop);
			mpEditEventDlg->SetCtrlNotifyFn(IDC_ALWAYS_ON_TOP, this,
				(GUI_CB_DlgNotifyFn)&CEditEvents::AlwaysOnTop);
			AlwaysOnTop(mpEditEventDlg, IDC_ALWAYS_ON_TOP);	// Execute to activate always on top property

			// Set up Close button action (bottom of 4 wide buttons)
			mpEditEventDlg->DefinePushBtn(IDCANCEL, GUI_CANCEL);
			mpEditEventDlg->SetDlgExitValidateFn(this, (GUI_CB_DlgExitFn)&CEditEvents::EditEventDlgExitValidateFn);

			// Action for top of the 4 wide buttons labeled either "Create New Event" or "Store New Event"
			mpEditEventDlg->DefinePushBtn(IDC_CREATE_NEW_EVENT, GUI_NONE);
			mpEditEventDlg->SetCtrlNotifyFn(IDC_CREATE_NEW_EVENT, this,
				(GUI_CB_DlgNotifyFn)&CEditEvents::CreateNewEvent);

			// Action for 2nd of the 4 wide buttons labeled either "Delete Current Event" or
			// "Cancel Current Edit" or " Cancel New Edit"
			mpEditEventDlg->DefinePushBtn(IDC_DELETE_CURRENT_EVENT, GUI_NONE);
			mpEditEventDlg->SetCtrlNotifyFn(IDC_DELETE_CURRENT_EVENT, this,
				(GUI_CB_DlgNotifyFn)&CEditEvents::DeleteCurrentEvent);

			// Action for 3rd of the 4 wide buttons labeled either "Edit Current Event" or "Store Edit Changes"
			mpEditEventDlg->DefinePushBtn(IDC_EDIT_CURRENT_EVENT, GUI_NONE);
			mpEditEventDlg->SetCtrlNotifyFn(IDC_EDIT_CURRENT_EVENT, this,
				(GUI_CB_DlgNotifyFn)&CEditEvents::EditCurrentEvent);

			mpEditEventDlg->DefinePushBtn(IDC_PREVIOUS_EVENT, GUI_NONE);
			mpEditEventDlg->SetCtrlNotifyFn(IDC_PREVIOUS_EVENT, this,
				(GUI_CB_DlgNotifyFn)&CEditEvents::GoToPrevEvent);

			mpEditEventDlg->DefinePushBtn(IDC_NEXT_EVENT, GUI_NONE);
			mpEditEventDlg->SetCtrlNotifyFn(IDC_NEXT_EVENT, this,
				(GUI_CB_DlgNotifyFn)&CEditEvents::GoToNextEvent);

			UINT uiNumVisibleTimeFlds = 3;	// Show hours, minutes and seconds.
			mpEditEventDlg->DefineFldTime(IDC_EVENT_START_TIME, &StartTime, uiNumVisibleTimeFlds);
			mpEditEventDlg->SetCtrlValidateFn(IDC_EVENT_START_TIME, this,
				(GUI_CB_DlgValidateFn)&CEditEvents::ValidateEventStartTimeChange);
			mpEditEventDlg->DefineFldTime(IDC_EVENT_END_TIME, &EndTime, uiNumVisibleTimeFlds);
			mpEditEventDlg->SetCtrlValidateFn(IDC_EVENT_END_TIME, this,
				(GUI_CB_DlgValidateFn)&CEditEvents::ValidateEventEndTimeChange);

			mpEditEventDlg->DefineFldTxt(IDC_EVENT_ID, &EventID, uiEVENT_ID_DIM);
			mpEditEventDlg->SetCtrlValidateFn(IDC_EVENT_ID, this,
				(GUI_CB_DlgValidateFn)&CEditEvents::ValidateEventIdChange);

			mpEditEventDlg->DefinePickList(IDC_EVENT_TYPE, naszList, &uiPickIndex);
			mpEditEventDlg->SetCtrlNotifyFn(IDC_EVENT_TYPE, this,
				(GUI_CB_DlgNotifyFn)&CEditEvents::NotifyEventTypeChange);

			mpEditEventDlg->DefineFldTxt(IDC_EVENT_CHANNEL, &EventChannel);
			// User is never allowed to edit channel field.
			mpEditEventDlg->SetEnable(IDC_EVENT_CHANNEL, FALSE);

			SetMode(EEM_NONE);

			mpEditEventDlg->GoModeless(TRUE);
		}
	}
}


void CEditEvents::SetFocusTo()
{
	if (mpEditEventDlg == NULL) {
		CreateDlg();
	}
	else {
		mpEditEventDlg->SetFocus();
	}
}



BOOL CEditEvents::ValidateEventIdChange(CGUI_Dlg *pDlg, RESOURCE_ID Id)
{
	BOOL bValid = TRUE;

	mbAnyEditChangesPending = TRUE;
	return(bValid);
}


BOOL CEditEvents::ValidateEventStartTimeChange(CGUI_Dlg *pDlg, RESOURCE_ID Id)
{
	BOOL bValid = TRUE;

	mbAnyEditChangesPending = TRUE;
	return(bValid);
}


BOOL CEditEvents::ValidateEventEndTimeChange(CGUI_Dlg *pDlg, RESOURCE_ID Id)
{
	BOOL bValid = TRUE;

	mbAnyEditChangesPending = TRUE;
	return(bValid);
}


void CEditEvents::NotifyEventTypeChange(CGUI_Dlg *pDlg, RESOURCE_ID Id)
{
	mbAnyEditChangesPending = TRUE;
}

// Decide which buttons to enable
void CEditEvents::SetEditEventBtnsEnable()
{
	BOOL bSuccess = FALSE;

	// Turn off all action buttons by default. On a case-by-case basis enable
	// them as appropriate for whatever mode we are entering.
	BOOL bEnable_IDCANCEL = FALSE;
	BOOL bEnable_IDC_CREATE_NEW_EVENT = FALSE;
	BOOL bEnable_IDC_DELETE_CURRENT_EVENT = FALSE;
	BOOL bEnable_IDC_EDIT_CURRENT_EVENT = FALSE;
	BOOL bEnable_IDC_PREVIOUS_EVENT = FALSE;
	BOOL bEnable_IDC_NEXT_EVENT = FALSE;

	BOOL bCursorInHiliteRegion = FALSE;

	if (mpEditEventDlg != NULL) {
		if (mpPlotPane != NULL) {
			UINT uiCurrentEventTraceIndex;
			COLORREF HiliteColor;
			double dValL;
			double dValR;
			bCursorInHiliteRegion = mpPlotPane->IsCursorInHiliteRegion(
				&uiCurrentEventTraceIndex, &dValL, &dValR, &HiliteColor);

			switch (meMode) {
				case EEM_ADD_NEW:	// just selected to "Edit New Events"
					bEnable_IDCANCEL = TRUE;	// enable "Close"
					bEnable_IDC_CREATE_NEW_EVENT = TRUE;	// Enable "Store New Event"
					bEnable_IDC_DELETE_CURRENT_EVENT = TRUE;	// Enable "Cancel New Event"
					break;

				case EEM_EDIT_PREV:	// just selected to "Edit Current Event"
					bEnable_IDCANCEL = TRUE;	// enable "Close"	
					bEnable_IDC_EDIT_CURRENT_EVENT = TRUE;	// Enable "Store Edit Changes"
					bEnable_IDC_DELETE_CURRENT_EVENT = TRUE;	// Enable "Cancel Current Edit"
					break;

				case EEM_DELETE_PREV:	// just selected to "Delete Current Event"
					bEnable_IDCANCEL = TRUE;	// enable "Close"
					break;

				case EEM_NONE:	// just brought up edit box or finished last edit
					if (bCursorInHiliteRegion) {	// in an event
						bEnable_IDC_CREATE_NEW_EVENT = FALSE;	// disable "Edit New Event"
						bEnable_IDC_DELETE_CURRENT_EVENT = TRUE;	// enable "Delete Current Event"
						bEnable_IDC_EDIT_CURRENT_EVENT = TRUE;	// enable "Edit Current Event"
					}
					else {	// not in a hilite retion
						bEnable_IDC_CREATE_NEW_EVENT = TRUE;	// enable "Edit New Event"
						bEnable_IDC_DELETE_CURRENT_EVENT = FALSE;	// disable "Delete Current Event"
						bEnable_IDC_EDIT_CURRENT_EVENT = FALSE;	// disable "Edit Current Event"
					}
					bEnable_IDCANCEL = TRUE;	// enable "Close"
					bEnable_IDC_PREVIOUS_EVENT = TRUE;	// enable "Previous"
					bEnable_IDC_NEXT_EVENT = TRUE;	// enable "Next"
					break;
			};

			mpEditEventDlg->SetEnable(IDCANCEL, bEnable_IDCANCEL);
			mpEditEventDlg->SetEnable(IDC_CREATE_NEW_EVENT, bEnable_IDC_CREATE_NEW_EVENT);
			mpEditEventDlg->SetEnable(IDC_DELETE_CURRENT_EVENT, bEnable_IDC_DELETE_CURRENT_EVENT);
			mpEditEventDlg->SetEnable(IDC_EDIT_CURRENT_EVENT, bEnable_IDC_EDIT_CURRENT_EVENT);
			mpEditEventDlg->SetEnable(IDC_PREVIOUS_EVENT, bEnable_IDC_PREVIOUS_EVENT);
			mpEditEventDlg->SetEnable(IDC_NEXT_EVENT, bEnable_IDC_NEXT_EVENT);

			// For previous-event and next-event btns only enable if bEnable_IDC_XXX_EVENT is
			// TRUE from above AND there is a previous (next) event to move to.
			if (bEnable_IDC_PREVIOUS_EVENT) {
				BOOL bPrevRegion = mpPlotPane->PrevHiliteRegion(FALSE);	// FALSE => don't move cursor.
				mpEditEventDlg->SetEnable(IDC_PREVIOUS_EVENT, bPrevRegion);
			}
			else {
				mpEditEventDlg->SetEnable(IDC_PREVIOUS_EVENT, FALSE);
			}

			if (bEnable_IDC_NEXT_EVENT) {
				BOOL bNextRegion = mpPlotPane->NextHiliteRegion(FALSE);	// FALSE => don't move cursor.
				mpEditEventDlg->SetEnable(IDC_NEXT_EVENT, bNextRegion);
			}
			else {
				mpEditEventDlg->SetEnable(IDC_NEXT_EVENT, FALSE);
			}
		}
	}
}

// Decide which button labels to use
BOOL CEditEvents::SetMode(EDIT_EVENTS_MODE eMode)
{
	BOOL bSuccess = FALSE;

	BOOL bVisible;
	BOOL bEnbResize;
	BOOL bEnbBoxNew;
	BOOL bEnbMove;

	BOOL bDisplayTypePickList = FALSE;

	// Disable all edit fields by default. On a case-by-case basis enable
	// them as appropriate for whatever mode we are entering.
	BOOL bEnable_IDC_EVENT_TYPE = FALSE;
	BOOL bEnable_IDC_EVENT_ID = FALSE;
	// don't allow user to edit begin / end times other than by dragging box
	// edges to minimize hassle for programmer and minimize chang\ce of
	// misundertanding about events having to begin and end on an actual data point.
	BOOL bEnable_IDC_EVENT_START_TIME = FALSE;
	BOOL bEnable_IDC_EVENT_END_TIME = FALSE;

	// Turn off all action buttons by default. On a case-by-case basis enable
	// them as appropriate for whatever mode we are entering.
	BOOL bEnable_IDCANCEL = FALSE;
	BOOL bEnable_IDC_CREATE_NEW_EVENT = FALSE;
	BOOL bEnable_IDC_DELETE_CURRENT_EVENT = FALSE;
	BOOL bEnable_IDC_EDIT_CURRENT_EVENT = FALSE;
	BOOL bEnable_IDC_PREVIOUS_EVENT = FALSE;
	BOOL bEnable_IDC_NEXT_EVENT = FALSE;

	// Set normal labels for action buttons then change as necessary below.
	const char *sz_IDC_CREATE_NEW_EVENT = "C&reate New Event...";
	const char *sz_IDC_DELETE_CURRENT_EVENT = "&Delete Current Event...";
	const char *sz_IDC_EDIT_CURRENT_EVENT = "&Edit Current Event...";

	BOOL bCursorInHiliteRegion = FALSE;
	if (mpEditEventDlg != NULL) {
		if (mpPlotPane != NULL) {
			UINT uiCurrentEventTraceIndex;
			COLORREF HiliteColor;
			double dValL;
			double dValR;
			bCursorInHiliteRegion = mpPlotPane->IsCursorInHiliteRegion(
				&uiCurrentEventTraceIndex, &dValL, &dValR, &HiliteColor);

			switch (eMode) {
				case EEM_ADD_NEW:	// selected "Create New Event"
					bSuccess = TRUE;
					bVisible = TRUE;
					bEnbResize = TRUE;
					bEnbBoxNew = FALSE;	// since the program creates a "starter" box.
					bEnbMove = FALSE;

					bDisplayTypePickList = TRUE;

					bEnable_IDCANCEL = TRUE;
					bEnable_IDC_CREATE_NEW_EVENT = TRUE;
					bEnable_IDC_DELETE_CURRENT_EVENT = TRUE;

					bEnable_IDC_EVENT_TYPE = TRUE;
					bEnable_IDC_EVENT_ID = TRUE;

					sz_IDC_CREATE_NEW_EVENT = "&Store New Event...";
					sz_IDC_DELETE_CURRENT_EVENT = "Cancel New E&vent...";
					break;

				case EEM_EDIT_PREV:	// selected "Edit Current Event"
					bSuccess = TRUE;
					bVisible = TRUE;
					bEnbResize = TRUE;
					bEnbBoxNew = FALSE;
					bEnbMove = FALSE;

					bEnable_IDC_EVENT_TYPE = TRUE;
					bEnable_IDC_EVENT_ID = TRUE;

					bEnable_IDCANCEL = TRUE;
					bEnable_IDC_EDIT_CURRENT_EVENT = TRUE;
					bEnable_IDC_DELETE_CURRENT_EVENT = TRUE;

					sz_IDC_DELETE_CURRENT_EVENT = "Cancel Current &Edit";
					sz_IDC_EDIT_CURRENT_EVENT = "&Store Edit Changes";
					break;

				case EEM_DELETE_PREV:	// selected "Delete Current Event"
					bSuccess = TRUE;
					bDisplayTypePickList = TRUE;
					bEnable_IDCANCEL = TRUE;
					break;

				case EEM_NONE:	// just brought up edit box or are finished with edit.
					bSuccess = TRUE;
					bVisible = FALSE;
					bEnbResize = FALSE;
					bEnbBoxNew = FALSE;
					bEnbMove = FALSE;

					if (bCursorInHiliteRegion) {
						bDisplayTypePickList = TRUE;
						bEnable_IDC_CREATE_NEW_EVENT = FALSE;
						bEnable_IDC_DELETE_CURRENT_EVENT = TRUE;
						bEnable_IDC_EDIT_CURRENT_EVENT = TRUE;
					}
					else {
						bEnable_IDC_CREATE_NEW_EVENT = TRUE;
						bEnable_IDC_DELETE_CURRENT_EVENT = FALSE;
						bEnable_IDC_EDIT_CURRENT_EVENT = FALSE;
					}
					bEnable_IDCANCEL = TRUE;
					bEnable_IDC_PREVIOUS_EVENT = TRUE;
					bEnable_IDC_NEXT_EVENT = TRUE;
					break;
			};

			if (bSuccess) {
				BOOL bModeChanged = (meMode != eMode);

				meMode = eMode;
				mpPlotPane->SetPlotObjOptions(bVisible, bEnbResize, bEnbBoxNew, bEnbMove);
				//mpPlotPane->InvalidateGraphDisplay();
				
				mpEditEventDlg->SetEnable(IDC_EVENT_TYPE, bEnable_IDC_EVENT_TYPE);
				if (bDisplayTypePickList) {
					double dDummyCursorX, dDummyCursorY;
					UINT uiPL_ChanIndex, uiCurrentEventTraceIndex, uiDummyPotentialTraceIndex;

					mpPlotPane->GetCursorPos(&dDummyCursorX, &dDummyCursorY, &uiCurrentEventTraceIndex);
					mpParentPlotDlg->TraceIndexToPotentialTraceIndex(uiCurrentEventTraceIndex, &uiDummyPotentialTraceIndex, &uiPL_ChanIndex);
					if (bCursorInHiliteRegion) {	// sfk
						UpdateDlgTypePickList(uiPL_ChanIndex, dValL, dValR);	//sfk
					}
					else {
						UpdateDlgTypePickList(uiPL_ChanIndex);
					}
				}

				UpdateDlg();

				mpEditEventDlg->SetEnable(IDC_EVENT_START_TIME, bEnable_IDC_EVENT_START_TIME);
				mpEditEventDlg->SetEnable(IDC_EVENT_END_TIME, bEnable_IDC_EVENT_END_TIME);
				mpEditEventDlg->SetEnable(IDC_EVENT_ID, bEnable_IDC_EVENT_ID);

				mpEditEventDlg->SetEnable(IDCANCEL, bEnable_IDCANCEL);
				mpEditEventDlg->SetEnable(IDC_CREATE_NEW_EVENT, bEnable_IDC_CREATE_NEW_EVENT);
				mpEditEventDlg->SetEnable(IDC_DELETE_CURRENT_EVENT, bEnable_IDC_DELETE_CURRENT_EVENT);
				mpEditEventDlg->SetEnable(IDC_EDIT_CURRENT_EVENT, bEnable_IDC_EDIT_CURRENT_EVENT);
				mpEditEventDlg->SetEnable(IDC_PREVIOUS_EVENT, bEnable_IDC_PREVIOUS_EVENT);
				mpEditEventDlg->SetEnable(IDC_NEXT_EVENT, bEnable_IDC_NEXT_EVENT);

				// The button text only needs to change when the mode changes and is a relatively
				// "expensive" operation check for mode change.
				if (bModeChanged) {
					mpEditEventDlg->SetText(IDC_CREATE_NEW_EVENT, sz_IDC_CREATE_NEW_EVENT);
					mpEditEventDlg->SetText(IDC_EDIT_CURRENT_EVENT, sz_IDC_EDIT_CURRENT_EVENT);
					mpEditEventDlg->SetText(IDC_DELETE_CURRENT_EVENT, sz_IDC_DELETE_CURRENT_EVENT);
				}

				// For previous-event and next-event btns only enable if bEnable_IDC_XXX_EVENT is
				// TRUE from above AND there is a previous (next) event to move to.
				if (bEnable_IDC_PREVIOUS_EVENT) {
					BOOL bPrevRegion = mpPlotPane->PrevHiliteRegion(FALSE);	// FALSE => don't move cursor.
					mpEditEventDlg->SetEnable(IDC_PREVIOUS_EVENT, bPrevRegion);
				}
				else {
					mpEditEventDlg->SetEnable(IDC_PREVIOUS_EVENT, FALSE);
				}

				if (bEnable_IDC_NEXT_EVENT) {
					BOOL bNextRegion = mpPlotPane->NextHiliteRegion(FALSE);	// FALSE => don't move cursor.
					mpEditEventDlg->SetEnable(IDC_NEXT_EVENT, bNextRegion);
				}
				else {
					mpEditEventDlg->SetEnable(IDC_NEXT_EVENT, FALSE);
				}
			}
		}
	}
	return(bSuccess);
}

// Put new values in the Start/End fields on the dialog box
// Change to DATE 08-29-2002
void CEditEvents::UpdateDlgTimes(double dLowValueRelDate, double dHighValueRelDate)
{
	GEN_DATE_STRUCT GenDate;
	GEN_TIME_STRUCT GenTime;

	if (mpEditEventDlg != NULL) {
		if ((dLowValueRelDate == 0.0) && (dHighValueRelDate == 0.0)) {
			mpEditEventDlg->SetText(IDC_EVENT_START_TIME, "");
			mpEditEventDlg->SetText(IDC_EVENT_END_TIME, "");
		}
		else {
			DATE dLowValueDate = mpPlotDB->RelDateToDate(dLowValueRelDate);
			//Date.DATETimestampToGenStructs(dLowValueDate, &GenDate, &GenTime);
			glMyDateTime.DATETimestampToGenStructs(dLowValueDate, &GenDate, &GenTime);
			mpEditEventDlg->FldTimeSetValue(IDC_EVENT_START_TIME, GenTime);

			DATE dHighValueDate = mpPlotDB->RelDateToDate(dHighValueRelDate);
			//Date.DATETimestampToGenStructs(dHighValueDate, &GenDate, &GenTime);
			glMyDateTime.DATETimestampToGenStructs(dHighValueDate, &GenDate, &GenTime);
			mpEditEventDlg->FldTimeSetValue(IDC_EVENT_END_TIME, GenTime);
		}
	}
}

// Change the choices for the picklist depending on the channel
void CEditEvents::UpdateDlgTypePickList(UINT uiPL_ChanIndex)
{
	mpEditEventDlg->PickListClear(IDC_EVENT_TYPE);

	struct IDStruct ChanID;
	pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID);

	char **pList;
	short sNumChcs;

	int i = GetEventTypePicklist(ChanID, GRAPH_LIST, &pList, &sNumChcs);
	if (i == TRUE) {
		for (short s = 0; s < sNumChcs; s++) {
			mpEditEventDlg->PickListAdd(IDC_EVENT_TYPE, pList[s]);
		}
		mpEditEventDlg->PickListSetValue(IDC_EVENT_TYPE, 0);
	}
}

// Change to DATE 08-29-2002						
void CEditEvents::UpdateDlgTypePickList(UINT uiPL_ChanIndex, double dLowValueRelDate, double dHighValueRelDate)
{
	struct db_event_rec dbChanEvent;
	DATE dRelDateFirst = dLowValueRelDate;
	DATE dRelDateLast = dHighValueRelDate;

	// Update the list based only on the channel.
	UpdateDlgTypePickList(uiPL_ChanIndex);

	// If we can find the event (defined by channel and time boundaries) set the type pick list selection
	// to be the currently selected type for that event.
	BOOL bSuccess = mpPlotDB->ReadEventRecord(uiPL_ChanIndex, dRelDateFirst, dRelDateLast, &dbChanEvent);
	if (bSuccess) 
	{
		UINT uiPickIndex;
		struct IDStruct ChanID;

		pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID);

		if (GetPlNumForEventTypes(ChanID, &uiPickIndex, dbChanEvent.us_event_type, dbChanEvent.us_event_type1) == TRUE) 
		{
			mpEditEventDlg->PickListSetValue(IDC_EVENT_TYPE, uiPickIndex);
		}
	}
}

	
// UpdateDlg is called:
//	1) Cursor moves.
//	2) SetMode is called.
//	3) An event is created or deleted.
void CEditEvents::UpdateDlg()
{
    UINT uiCurrentEventTraceIndex;
    BOOL bCursorInHiliteRegion;
    UINT uiPL_ChanIndex;
    UINT uiDummyPotentialTraceIndex;
    double dLowValueRelSecs, dHighValueRelSecs;
    COLORREF HiliteColor;
	CGStr Label;
	CGStr ChanName;
	double dDummyCursorX, dDummyCursorY;

	if (mpParentPlotDlg && mpEditEventDlg && mpPlotDB && mpPlotPane) 
	{
		bCursorInHiliteRegion = 
			mpPlotPane->IsCursorInHiliteRegion(
			&uiCurrentEventTraceIndex,
			&dLowValueRelSecs, 
			&dHighValueRelSecs, 
			&HiliteColor, 
			&Label);

		mpPlotPane->GetCursorPos(&dDummyCursorX, &dDummyCursorY, &uiCurrentEventTraceIndex);
		mpParentPlotDlg->TraceIndexToPotentialTraceIndex(
			uiCurrentEventTraceIndex, &uiDummyPotentialTraceIndex, &uiPL_ChanIndex);
		// ?????????????????????????????? AreaIndex or PL_Index
		mpPlotDB->GetDetName(uiPL_ChanIndex, &ChanName);

		mpEditEventDlg->FldTxtSetValue(IDC_EVENT_CHANNEL, ChanName);

		// If the crosshair is within a hilite region or we are in the process of creating or editing a hilite region...
		if ((bCursorInHiliteRegion) || (meMode == EEM_ADD_NEW) || (meMode == EEM_EDIT_PREV)) {
			if (bCursorInHiliteRegion) {
				UpdateDlgTimes(dLowValueRelSecs, dHighValueRelSecs);
			}

			// Don't update ID and type selection if we are in the middle of an edit / add.
			if ((meMode != EEM_ADD_NEW) && (meMode != EEM_EDIT_PREV)) 
			{
				mpEditEventDlg->FldTxtSetValue(IDC_EVENT_ID, Label);

				if ((mdPrevUpdateDlgCursorLowValueRelSecs != dLowValueRelSecs) ||
					(mdPrevUpdateDlgCursorHighValueRelSecs != dHighValueRelSecs) ||
					(muiPrevUpdateDlgCursorTraceIndex != uiCurrentEventTraceIndex)) 
				{
						mdPrevUpdateDlgCursorLowValueRelSecs = dLowValueRelSecs;
						mdPrevUpdateDlgCursorHighValueRelSecs = dHighValueRelSecs;
						muiPrevUpdateDlgCursorTraceIndex = uiCurrentEventTraceIndex;
						UpdateDlgTypePickList(uiPL_ChanIndex, dLowValueRelSecs, dHighValueRelSecs);  
				}
			}
		}
		else 
		{ 
			UpdateDlgTimes();

			mdPrevUpdateDlgCursorLowValueRelSecs = 0.0;
			mdPrevUpdateDlgCursorHighValueRelSecs = 0.0;
			muiPrevUpdateDlgCursorTraceIndex = uiCurrentEventTraceIndex;

			mpEditEventDlg->SetEnable(IDC_EVENT_START_TIME, FALSE);
			mpEditEventDlg->SetEnable(IDC_EVENT_END_TIME, FALSE);

			mpEditEventDlg->FldTxtSetValue(IDC_EVENT_ID, "");
			mpEditEventDlg->SetEnable(IDC_EVENT_ID, FALSE);

			mpEditEventDlg->PickListClear(IDC_EVENT_TYPE);
			mpEditEventDlg->FldTxtSetValue(IDC_EVENT_TYPE, "");
			mpEditEventDlg->SetEnable(IDC_EVENT_TYPE, FALSE);
		}
		SetEditEventBtnsEnable();
	}
	DisplayEmphasizedTrace(muiEmphasizedTraceIndex, mdActualMinEmphasizedX, mdActualMaxEmphasizedX);
}


	
// Action for "Delete Current Event" (mode EEM_NONE) or "Cancel New Event" (mode EEM_ADD_NEW) or
// "Cancel Current Edit" (mode EEM_EDIT_PREV)
// Cahnged to DATE 08-20-2002
void CEditEvents::DeleteCurrentEvent(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    BOOL bCursorInHiliteRegion;
    UINT uiPL_ChanIndex, uiDummyPotentialTraceIndex;
	UINT uiCurrentEventTraceIndex;
    COLORREF HiliteColor;
	CGStr Label;
    DATE dLowValueRelDate, dHighValueRelDate;

	if (mpParentPlotDlg != NULL) {
		if (mpPlotDB != NULL) {
			switch (meMode) {
				case EEM_EDIT_PREV:	// "Cancel Current Event"
					{
						// If we are in edit-previous-event mode the "Delete Current Event" button has been
						// re-labeled "Cancel Event changes" and will cause the old event parameters to be re-established.
						FinishEventEdit(FALSE);	// FALSE => Don't save changes.
					}
					break;
				case EEM_ADD_NEW:	//"Cancel New Event"
					{
						// If we are in create-new-event mode the "Delete Current Event" button has been
						// re-labeled "Cancel New Event" and will cause the new event to be dismissed.
						BOOL bSaveNewEvent = FALSE;
						FinishCreateNewEvent(bSaveNewEvent);
					}
					break;
				case EEM_NONE:	// "Delete Current Event"
					bCursorInHiliteRegion = mpPlotPane->IsCursorInHiliteRegion(&uiCurrentEventTraceIndex,
						&dLowValueRelDate, &dHighValueRelDate, &HiliteColor, &Label);
				
					if (bCursorInHiliteRegion) {
						CGFmtStr Msg;
						CGStr ChanName;
						CGStr EventTypeName;
						struct db_event_rec dbChanEvent;
						UINT uiPickIndex;

						if (mpParentPlotDlg != NULL) {
							mpParentPlotDlg->TraceIndexToPotentialTraceIndex(uiCurrentEventTraceIndex,
								&uiDummyPotentialTraceIndex, &uiPL_ChanIndex);
							if (mpPlotDB != NULL) {
								mpPlotDB->GetDetName(uiPL_ChanIndex, &ChanName);

								DATE dRelDateFirst = dLowValueRelDate;
								DATE dRelDateLast = dHighValueRelDate;

								BOOL bSuccess = mpPlotDB->ReadEventRecord(uiPL_ChanIndex, dRelDateFirst, dRelDateLast, &dbChanEvent);
								if (bSuccess) {
									struct IDStruct ChanID;
									pglFacCfg->GetChannelIDForPicklistChoice((short)uiPL_ChanIndex, &ChanID);

									if (GetPlNumForEventTypes(ChanID, &uiPickIndex, dbChanEvent.us_event_type, dbChanEvent.us_event_type1) == TRUE) {
										char **pList;
										short sNumChcs;

										int i = GetEventTypePicklist(ChanID, GRAPH_LIST, &pList, &sNumChcs);
										if (i == TRUE) {
											if ((short)uiPickIndex < sNumChcs) {
												EventTypeName = pList[uiPickIndex];
											}
										}
									}
								}
							}
						}

						SetMode(EEM_DELETE_PREV);

						if (ChanName.GetDim() > 1) {
							Msg.Printf("Delete channel \"%s\" event\n", ChanName.Get_sz());
						}
						else {
							Msg.Printf("Delete channel event\n", ChanName.Get_sz());
						}
						
						if (EventTypeName.GetDim() > 1) {
							EventTypeName.StripLeadAndTrail();
							Msg.PrintfAppend("of type \"%s\" ", EventTypeName.Get_sz());
						}

						if (Label.GetDim() > 1) {
							Msg.PrintfAppend("labeled, \"%s\", ", Label.Get_sz());
						}

						Msg += "\n";
						Msg.PrintfAppend("spanning: %s", mpParentPlotDlg->RelDateToDateAndTimeStr(dLowValueRelDate));
						Msg.PrintfAppend(" - %s?", mpParentPlotDlg->RelDateToDateAndTimeStr(dHighValueRelDate));

						// Momentarily emphasize the part of the trace containing the event which is being deleted.
						mbEmphasizeActive = TRUE;
						muiEmphasizedTraceIndex = uiCurrentEventTraceIndex;
						DisplayEmphasizedTrace(uiCurrentEventTraceIndex, dLowValueRelDate, dHighValueRelDate);

//int i = AfxMessageBox(Msg.Get_sz(), MB_YESNO | MB_TASKMODAL | MB_ICONQUESTION);
// Accelerator
						//						HWND hPrevFocus = CGUI_App::SetFocus(NULL);	// hAppWnd = NULL => this application; returns prev focus window.

						GUI_ACTION Return = GUI_MsgBox(Msg, GUI_ICON_QUESTION, GUI_YESNO);
						mbEmphasizeActive = FALSE;

						if (Return == GUI_YES) {
							if (mpParentPlotDlg->TraceIndexToPotentialTraceIndex(uiCurrentEventTraceIndex, &uiDummyPotentialTraceIndex, &uiPL_ChanIndex)) {
								mpPlotDB->DeleteEventRecord(uiPL_ChanIndex, dLowValueRelDate, dHighValueRelDate);
								mpParentPlotDlg->UpdateAxisEvents(uiCurrentEventTraceIndex);		//16-Dec-2004 added this to force a new read of events after deleted one
								mpParentPlotDlg->DeleteScreenEvent(uiCurrentEventTraceIndex, dLowValueRelDate, TRUE);	// TRUE => redraw graph to delete event hilite on screen.

							}
						}
						else {
							// If DeleteScreenEvent is called it will cause the screen to update.
							// However if the delete is cancelled we need to repaint the screen to
							// get rid of the EmphasizeRegion() action.
							if (mpPlotPane != NULL) mpPlotPane->InvalidateGraphDisplay();
						}

					}
					break;
			}

			SetMode(EEM_NONE);
		}
	}
}


// Action for Close button in Edit Events dialog box.
BOOL CEditEvents::EditEventDlgExitValidateFn(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction)
{
	BOOL bOkToClose = TRUE;  // We never block the closing; we just want to know about it,
	
	// We also want to make sure user finishes any event create or edit that might be in progress.
	switch (meMode) {
		case EEM_ADD_NEW:
			{
				CGFmtStr Msg;
				BOOL bSaveNewEvent;

				Msg = "A new event has been\ndefined but not stored.\n\nDo you want to store the new event?";
				GUI_ACTION Return = GUI_MsgBox(Msg, GUI_ICON_EXCLAMATION, GUI_YESNO);
				if (Return == GUI_YES) {
					bSaveNewEvent = TRUE;
				}
				else {
					bSaveNewEvent = FALSE;
				}
				FinishCreateNewEvent(bSaveNewEvent);
			}
			break;
		case EEM_EDIT_PREV:
			{
				CGFmtStr Msg;
				BOOL bSaveChanges;

				Msg = "Modifications to the current\nevent have not been stored.\n\nDo you want to store the event changes?";

				GUI_ACTION Return = GUI_MsgBox(Msg, GUI_ICON_EXCLAMATION, GUI_YESNO);
				if (Return == GUI_YES) {
					bSaveChanges = TRUE;
				}
				else {
					bSaveChanges = FALSE;
				}
				FinishEventEdit(bSaveChanges);
			}
			break;
	}

	mpEditEventDlg = NULL;	// so that we don't try to close it in CEditEvents destructor.
							// but after all of the above cleanup is finished.
	return(bOkToClose);
}


// Action for Edit Current Event button (This button active when select Edit Event from main dialog
// and you are in a hilite region.
void CEditEvents::StartEventEdit()
{
	// Create "edit box" object on graph.
	if (mpPlotPane != NULL) 
	{
		UINT uiCurrentEventTraceIndex;
		BOOL bCursorInHiliteRegion;
		COLORREF HiliteColor;
		double dValLeft, dValRight;

		bCursorInHiliteRegion = mpPlotPane->IsCursorInHiliteRegion(&uiCurrentEventTraceIndex,
				&dValLeft, &dValRight, &HiliteColor);
	
		if (bCursorInHiliteRegion) {
			mbAnyEditChangesPending = FALSE;

			// draw the box and set the possible box limits mdMinNewEventX and mdMaxNewEventX
			if (CreateEventBox(uiCurrentEventTraceIndex, dValLeft, dValRight)) {
				SetMode(EEM_EDIT_PREV);
			}
		}
	}
}


// Action for 3rd button in Edit Event dialog box.  Is either labeled "Edit Current Event" (mode EEM_NONE)
// or "Store Edit Changes" (mode EEM_EDIT_PREV)
void CEditEvents::EditCurrentEvent(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	// If we are in edit mode then the edit button has been re-labeled
	// as "Store Edit Changes" so do so.
	if (meMode == EEM_EDIT_PREV) // action for "Store Edit Changes
		FinishEventEdit(TRUE);	// TRUE => Save changes.
	else
		StartEventEdit();	// action for "Edit Current Event"
}


void CEditEvents::AlwaysOnTop(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	BOOL bAlwaysOnTop;

	pDlg->CheckBoxGetValue(IDC_ALWAYS_ON_TOP, &bAlwaysOnTop);
	if (bAlwaysOnTop) {
		::SetWindowPos(mpEditEventDlg->GetWndHandle(), HWND_TOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
	}
	else {
		::SetWindowPos(mpEditEventDlg->GetWndHandle(), HWND_NOTOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE);
	}

}


void CEditEvents::GoToPrevEvent(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	SetMode(EEM_NONE);
	if (mpPlotPane != NULL) {
		mpPlotPane->PrevHiliteRegion();
		UpdateDlg();
	}
	// If we don't set focus back to the "previous" button it will respond to Alt-N
	// but none of the other dialog buttons will respond to the Alt-HotKey. ?????
	pDlg->SetFocus(CtrlID);
}

void CEditEvents::GoToNextEvent(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
	SetMode(EEM_NONE);
	if (mpPlotPane != NULL) {
		mpPlotPane->NextHiliteRegion();
		UpdateDlg();
	}
	// If we don't set focus back to the "next" button it will respond to Alt-N
	// but none of the other dialog buttons will respond to the Alt-HotKey. ?????
	pDlg->SetFocus(CtrlID);
}


// Action for "Store New Event" (EEM_ADD_NEW mode) or action for "Cancel Current Edit" 
// Change to DATE 08-29-2002
void CEditEvents::FinishCreateNewEvent(BOOL bSaveNewEvent)
{
	double dBoxLeft, dBoxRight;

	if (DeletePlObjBox(&dBoxLeft, &dBoxRight, !bSaveNewEvent)) {
		if (bSaveNewEvent) {
			// Save new event.
			if (mpEditEventDlg != NULL) {
				// Make sure type is specified.
				UINT uiTypePickIndex;
				CGStr EventId;

				mpEditEventDlg->PickListGetValue(IDC_EVENT_TYPE, &uiTypePickIndex);
				mpEditEventDlg->FldTxtGetValue(IDC_EVENT_ID, &EventId);

				if (mpPlotDB != NULL) {
					UINT uiTraceIndex, uiPL_ChanIndex, uiDummyPotentialTraceIndex;
					double dDummyCursorX, dDummyCursorY;

					mpPlotPane->GetCursorPos(&dDummyCursorX, &dDummyCursorY, &uiTraceIndex);
					DATE dRelDateFirst = dBoxLeft;
					DATE dRelDateLast = dBoxRight;

					if (mpParentPlotDlg->TraceIndexToPotentialTraceIndex(uiTraceIndex, &uiDummyPotentialTraceIndex, &uiPL_ChanIndex)) {
						mpPlotDB->CreateEventRecord(uiPL_ChanIndex, dRelDateFirst, dRelDateLast, EventId, uiTypePickIndex);
						mpParentPlotDlg->UpdateAxisEvents(uiTraceIndex);
					}
				}
			}
		}
		else {
			if (mpPlotPane != NULL) mpPlotPane->InvalidateGraphDisplay();
		}
		SetMode(EEM_NONE);
	}
}


// Action for "Create New Event" (mode EEM_NONE) or "Store New Event" (mode EEM_ADD_NEW)
void CEditEvents::CreateNewEvent(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID)
{
    BOOL bCursorInHiliteRegion;
	UINT uiCurrentEventTraceIndex;
    COLORREF HiliteColor;
    double dLowValueRelSecs, dHighValueRelSecs;

	if (mpPlotDB != NULL) {
		// If we are in create-new-event-mode the "Create New Event" button has been
		// re-labeled "Store New Event" and will cause the new event to be stored.
		if (meMode == EEM_ADD_NEW) {	// "Store New Event"
			BOOL bSaveNewEvent = TRUE;
			FinishCreateNewEvent(bSaveNewEvent);
		}
		else {
			// this should never happen
			bCursorInHiliteRegion = mpPlotPane->IsCursorInHiliteRegion(&uiCurrentEventTraceIndex,
				&dLowValueRelSecs, &dHighValueRelSecs, &HiliteColor);
			
			if (bCursorInHiliteRegion) {
				GUI_MsgBox("Place cursor at beginning of new event\nthen select \"Create New Event\".\n\nA new event cannot begin within\nan existing event.");
			}
			else { //"Create New Event"
				double dHalfInitWidth = 40.0;
				double dCursorX, dCursorY;
				UINT uiAreaIndex;

				// get limits of how much is visible in the graph window
				if (mpPlotPane != NULL) {
					double dMinFullX, dMaxFullX, dMinVisibleX, dMaxVisibleX;
					mpPlotPane->GetLimitsX(&dMinFullX, &dMaxFullX, &dMinVisibleX, &dMaxVisibleX);
					dHalfInitWidth = (dMaxVisibleX - dMinVisibleX) * 0.1;	// change guess size to 10% sfk
				}

				mpPlotPane->GetCursorPos(&dCursorX, &dCursorY, &uiAreaIndex);	// save cursor position

				// figure out a guess for the box edges, snap the guess values to points // sfk
				double dValLeft = dCursorX - dHalfInitWidth;
				BOOL bOnNearestDataPt = TRUE;
				BOOL bUpdateDisplay = TRUE;
				mpPlotPane->SetCursorPos(dValLeft, uiAreaIndex, bOnNearestDataPt, bUpdateDisplay); //sfk
				mpPlotPane->GetCursorPos(&dValLeft);	//sfk
				double dValRight = dCursorX + dHalfInitWidth;
				mpPlotPane->SetCursorPos(dValRight, uiAreaIndex, bOnNearestDataPt, bUpdateDisplay); //sfk
				mpPlotPane->GetCursorPos(&dValRight);	//sfk
				mpPlotPane->SetCursorPos(dCursorX, uiAreaIndex);  // sfk

				// further limit how big box can be based on other hilite regions
				double dMinNewEventX, dMaxNewEventX;	// these are the limits, dValLeft and dValRight
														// are the limit points of the original box drawn
				DetermineEventLimits(uiAreaIndex, &dMinNewEventX, &dMaxNewEventX);
				if (dValLeft < dMinNewEventX) dValLeft = dMinNewEventX;
				if (dValRight > dMaxNewEventX) dValRight = dMaxNewEventX;

				if ((dValRight <= dValLeft) || (dMaxNewEventX <= dMinNewEventX)) {	// sfk added check of Min, Max X
					GUI_MsgBox("Cannot create event at this position\nbecause of potential overlapping adjacent events.");
				}
				else {
					if (CreateEventBox(uiAreaIndex, dValLeft, dValRight)) {
						SetMode(EEM_ADD_NEW);
					}
				}
			}
		}
	}
}


// Action for "Store Edit Changes" (EEM_PREV_MODE) or called by "Cancel Current Edit"
// Changed to support DATE 08-30-2002
void CEditEvents::FinishEventEdit(BOOL bSaveChanges)
{
	double dBoxLeft, dBoxRight;
	if (DeletePlObjBox(&dBoxLeft, &dBoxRight, !bSaveChanges)) {
		if (mpParentPlotDlg != NULL) {
			if (bSaveChanges) {
				if (mpEditEventDlg != NULL) {
					CGStr EventId, PickedItemText;
					UINT uiTypePickIndex;

					mpEditEventDlg->FldTxtGetValue(IDC_EVENT_ID, &EventId);
					mpEditEventDlg->PickListGetValue(IDC_EVENT_TYPE, &uiTypePickIndex, &PickedItemText);


					if (mpPlotDB != NULL) {
						UINT uiTraceIndex, uiPL_ChanIndex, uiDummyPotentialTraceIndex;
						double dLowValueRelSecs, dHighValueRelSecs;
						COLORREF HiliteColor;
						CGStr Label;

						BOOL bCursorInHiliteRegion = mpPlotPane->IsCursorInHiliteRegion(&uiTraceIndex,
								&dLowValueRelSecs, &dHighValueRelSecs, &HiliteColor, &Label);
						DATE dPrevLowValueRelSecs = dLowValueRelSecs;
						DATE dPrevHighValueRelSecs = dHighValueRelSecs;


						if (bCursorInHiliteRegion) {
							DATE dNewLowValueRelSecs = dBoxLeft;
							DATE dNewHighValueRelSecs = dBoxRight;

							if (mpParentPlotDlg->TraceIndexToPotentialTraceIndex(uiTraceIndex, &uiDummyPotentialTraceIndex, &uiPL_ChanIndex)) {
								BOOL bSuccess = mpPlotDB->EditEventRecord(uiPL_ChanIndex, dPrevLowValueRelSecs, dPrevHighValueRelSecs,
									dNewLowValueRelSecs, dNewHighValueRelSecs, EventId, uiTypePickIndex);
								mpParentPlotDlg->UpdateAxisEvents(uiTraceIndex);

							}
						}
					}
				}
			}
			else {
				if (mpPlotPane != NULL) mpPlotPane->InvalidateGraphDisplay();
			
			}
		}
		SetMode(EEM_NONE);
	}	
}



//////////////////////////////////////////////////////////////////////////////////////
// Event Box Routines
		
//10-Nov-2004 SFK	3.0.3.15 Did check that if the left and right values were equal, put the right
//					value 5 secs further right.  This allows editing of 1 channel events.
BOOL CEditEvents::CreateEventBox(UINT uiTraceIndex, double dValLeft, double dValRight)
{
	BOOL bSuccess = FALSE;
	
	double dNewValRight = dValRight;
	if (dValRight == dValLeft) dNewValRight = dValRight + 5.0/86400.0;	// add 5 sec if the same

	// Create "edit box" object on graph.
	if (mpPlotPane != NULL) {
		if (mpPlObjBox != NULL) {
			delete mpPlObjBox;
			mpPlObjBox = NULL;
		}

		PL_CTX *pPlCtx = mpPlotPane->GetPlotContext();
		BOOL bNoCallback = TRUE;
		BOOL bSelect = TRUE;
		BOOL bHide = FALSE;
		COLORREF BoxColor = GUI_MAGENTA;
		BOOL bAutoScale;

		mpPlotPane->GetLimitsY(uiTraceIndex, &bAutoScale, &mdEditBoxBottom, &mdEditBoxTop);
		// PlObjBoxCreateSimple works on currently selected area so select the proper area.
		mpPlotPane->SetCurrentArea(uiTraceIndex);
		int iStatus = PlObjBoxCreateSimple(pPlCtx, bNoCallback, &mpPlObjBox, bSelect, bHide,
			dValLeft, mdEditBoxBottom, dNewValRight, mdEditBoxTop, BoxColor);

		if (iStatus == PL_OK) {
			bSuccess = TRUE;
			mdLastBoxLeft = dValLeft;
			mdLastBoxRight = dNewValRight;

			UpdateDlgTimes(dValLeft, dNewValRight);

			float fLineThicknessPts = 5;

			mbEmphasizeActive = TRUE;
			DisplayEmphasizedTrace(uiTraceIndex, dValLeft, dNewValRight);
		}
		else {
			CGFmtStr Msg;

			//if (dValLeft == dValRight) {
			//	Msg.Printf("Cannot edit an event of only one channel.  Delete event and create a new one.");
			//}
			//else {
				Msg.Printf("Failed status (%d) returned from\nPlObjBoxCreateSimple in CEditEvents::CreateEventBox\nLeft=%lf  Bot=%lf  \nRight=%lf  Top=%lf",
					iStatus, dValLeft, mdEditBoxBottom, dNewValRight, mdEditBoxTop);
			//}
			GUI_MsgBox(Msg);
		}

		DetermineEventLimits(uiTraceIndex, &mdMinNewEventX, &mdMaxNewEventX);
	}
	return(bSuccess);
}


void CEditEvents::ResetEventBoxHeight()
{
	BOOL bCancel = TRUE;
	double dCursorX;
	double dBoxLeft, dBoxRight;
	double dYTop, dYBot;
	BOOL bAutoScale;

	if (mbEmphasizeActive) {
		if (mpPlotPane != NULL) {
			if (mpPlObjBox != NULL) {
				// Determine if the edit box top and bottom are different from display min, max.
				mpPlotPane->GetLimitsY(muiEmphasizedTraceIndex, &bAutoScale, &dYBot, &dYTop);
				if ((dYBot != mdEditBoxBottom) || (dYTop != mdEditBoxTop)) {
					dCursorX = (mdActualMinEmphasizedX + mdActualMaxEmphasizedX) / 2.0;
					mpPlotPane->SetCursorPos(dCursorX, muiEmphasizedTraceIndex);

					if (DeletePlObjBox(&dBoxLeft, &dBoxRight, bCancel)) {
						dCursorX = (mdActualMinEmphasizedX + mdActualMaxEmphasizedX) / 2.0;
						mpPlotPane->SetCursorPos(dCursorX, muiEmphasizedTraceIndex);
						// Now create a new one of the proper height.
						CreateEventBox(muiEmphasizedTraceIndex, mdActualMinEmphasizedX, mdActualMaxEmphasizedX);
					}
				}
			}
		}
	}
}


BOOL CEditEvents::DeletePlObjBox(double *pdBoxLeft, double *pdBoxRight, BOOL bCancel)
{
	BOOL bSuccess = FALSE;

	// Turn off emphasis flag. Note: This does not repaint the emphasized region.
	mbEmphasizeActive = FALSE;

	if (mpPlotPane != NULL) {
		if (mpPlObjBox != NULL) {
			bSuccess = TRUE;

			if (!bCancel) {
				double dBoxLeft = mpPlObjBox->aSides[0].dVal;
				double dBoxBottom = mpPlObjBox->aSides[1].dVal;
				double dBoxRight = mpPlObjBox->aSides[2].dVal;
				double dBoxTop = mpPlObjBox->aSides[3].dVal;

				UINT uiBoxTraceIndex = mpPlObjBox->aSides[3].iRowCol;

				// Find data points just outside of box boundaries.
				double dCursorX, dCursorY;
				UINT uiAreaIndex;

				// Remember cursor's original position.
				mpPlotPane->GetCursorPos(&dCursorX, &dCursorY, &uiAreaIndex);

				BOOL bOnNearestDataPt = TRUE;
				BOOL bUpdateDisplay = FALSE;
				// SetCursorPos for each edge with bOnNearestDataPt = TRUE then get actual cursor value...
				mpPlotPane->SetCursorPos(dBoxLeft, uiBoxTraceIndex, bOnNearestDataPt, bUpdateDisplay);
				mpPlotPane->GetCursorPos(pdBoxLeft);
				mpPlotPane->SetCursorPos(dBoxRight, uiBoxTraceIndex, bOnNearestDataPt, bUpdateDisplay);
				mpPlotPane->GetCursorPos(pdBoxRight);

//sfk			BOOL bBadExtent = FALSE;
//				if (*pdBoxLeft < mdMinNewEventX) {
//					*pdBoxLeft = mdMinNewEventX;
//					bBadExtent = TRUE;
//				}
//
//				if (*pdBoxRight > mdMaxNewEventX) {
//					*pdBoxRight = mdMaxNewEventX;
//					bBadExtent = TRUE;
//				}
//				if (bBadExtent) {
//					bSuccess = FALSE;
//					GUI_MsgBox("Events may not overlap other events.\n\nPlease modify the boundaries of\nthe new event.");
//				}

				// Now set cursor back to original position.
				mpPlotPane->SetCursorPos(dCursorX, uiAreaIndex);
			}
			mpPlotPane->DeletePlObj();
			mpPlObjBox = NULL;
		}
	}
	return(bSuccess);
}


void CEditEvents::DisplayEmphasizedTrace(UINT uiTraceIndex, double dValLeft, double dValRight)
{
	if (mpParentPlotDlg && mbEmphasizeActive && mpPlotPane) 
	{
		BOOL bNearestActualPt = TRUE;

		muiEmphasizedTraceIndex = uiTraceIndex;
		mpPlotPane->EmphasizeRegion(
			muiEmphasizedTraceIndex, 
			dValLeft, 
			dValRight, 
			bNearestActualPt,
			mEmphasizedColor, 
			mfEmphasizedLineThicknessPts, 
			&mdActualMinEmphasizedX, 
			&mdActualMaxEmphasizedX);
	}
}


int CEditEvents::EventBoxEdgeMoveNotify(double dLeft, double dRight, double dTop, double dBottom)
{
	int iRetVal = 1;	// Normal processing of box edge movement.

	// If the region selection highlighting is active check that we are within proper bounds.
	// If not set return value to 0 so the plot library will not allow the bounding box plot
	// object to be left at the too-large size.
	// Also adjust the newly requested bounds to be within the limits before emphasizing that
	// region of the plot below in calls to mpPlotPane->EmphasizeRegion().
	if ((mbEmphasizeActive) && ((dLeft < mdMinNewEventX) || (dRight > mdMaxNewEventX))) {
		BOOL bLeftButtonDown = TRUE;
		if (mpPlotPane != NULL) {
			PL_CTX *pPlCtx = mpPlotPane->GetPlotContext();
			if (pPlCtx != NULL) {
				PL_MOUSE *pMouse = pPlCtx->pMouse;
				if (pMouse != NULL) {
					bLeftButtonDown = pMouse->bLeftDown;
				}
			}
		}

		double dLeftLimit = mdMinNewEventX;
		double dRightLimit = mdMaxNewEventX;

		// Since this function is called repeatedly during a drag we need to guard against displaying 
		// the message until the user has terminated the drag; i.e. released the mouse button.
		if (!bLeftButtonDown) {
			GUI_MsgBox("Events cannot overlap adjacent events \nor extend beyond displayed data.");

			// If we return iRetVal = FALSE the plot box will "snap back" to the size it was before this
			// darg-sizing operation. It is this size that the new dLeft and dRight should conform to not
			// the originally established mdMinNewEventX and mdMaxNewEventX.
			// Only do this when the mouse drag button is released.

			if (mpPlObjBox != NULL) {
				double dBoxLeft = mpPlObjBox->aSides[0].dVal;
				double dBoxRight = mpPlObjBox->aSides[2].dVal;

				if ((dBoxLeft == dRight) || (dBoxRight == dLeft)) {
					// If the user crossed one edge over the other we have a special case.
					dLeft = dLeftLimit = dBoxLeft;
					dRight = dRightLimit = dBoxRight;
				}
				else {
					// Otherwise just restrict things to the tighter of the prev box  or orig limits.
					if (dLeftLimit < dBoxLeft) {
						dLeftLimit = dBoxLeft;
					}
					if (dRightLimit > dBoxRight) {
						dRightLimit = dBoxRight;
					}
				}
			}
		}

		// By enforcing the mdMinNewEventX and mdMaxNewEventX before drawing the emphasized trace we
		// assure ourselves that any adjacent events are never over written with an emphasized
		// trace so that we don't have to worry about constantly repainting them.
		if (dLeft < dLeftLimit) {
			dLeft = dLeftLimit;
		}
		if (dRight > dRightLimit) {
			dRight = dRightLimit;
		}

		iRetVal = 0;
	}

	if (mbEmphasizeActive) {
		// If both left & right edges have moved the box is moving vs. being resized so skip the
		// highlighting. It is possible for the user to "interchange" right & left by dragging one
		// edge over the other so we need to check that below.
		if ((mdLastBoxLeft != dLeft) && (mdLastBoxRight != dRight) &&
			(mdLastBoxLeft != dRight) && (mdLastBoxRight != dLeft)) {
//			GUI_Beep();
		}
		else {
			if (mpPlotPane != NULL) {
				// Erase old emphasized region first if new region is smaller on either end.
				if ((mdActualMinEmphasizedX < dLeft) || (mdActualMaxEmphasizedX > dRight)) {
					COLORREF Color = CPlotPane::GetTraceColor();
					float fLineThicknessPts = CPlotPane::GetLinePtSize();
					BOOL bNearestActualPt = TRUE;

					// Need to paint over emphasized region in plot bkg color in width of emphasized
					// region to "erase" emphasized trace before redrawing it.
					mpPlotPane->EmphasizeRegion(muiEmphasizedTraceIndex, mdActualMinEmphasizedX, mdActualMaxEmphasizedX,
						bNearestActualPt, CPlotPane::GetGraphBkgColor(), mfEmphasizedLineThicknessPts);

					// Now redraw it in "default" color and width.
					Color = CPlotPane::GetTraceColor();
					fLineThicknessPts = CPlotPane::GetLinePtSize();
					mpPlotPane->EmphasizeRegion(muiEmphasizedTraceIndex, mdActualMinEmphasizedX, mdActualMaxEmphasizedX,
						bNearestActualPt, Color, fLineThicknessPts);
				}
				// Emphasize new region.
				DisplayEmphasizedTrace(muiEmphasizedTraceIndex, dLeft, dRight);
			}
		}
	}

	// By placing the bounds checking and adjusting above the call to UpdateDlgTimes the times
	// echoed in the event details dialog are also forced to be within bounds. This has
	// the disadvantage of the user not getting feedback in the dialog box controls while
	// dragging the bounding box borders outside of the allowed region.

//	UpdateDlgTimes(dLeft, dRight);

	// Also check that the top / bottom of edit box have not been moved -- if they have move them back
	// to full scale.

	if (dBottom != mdEditBoxBottom) {
		iRetVal = 0;
		BOOL bLeftButtonDown = TRUE;
		if (mpPlotPane != NULL) {
			PL_CTX *pPlCtx = mpPlotPane->GetPlotContext();
			if (pPlCtx != NULL) {
				PL_MOUSE *pMouse = pPlCtx->pMouse;
				if (pMouse != NULL) {
					bLeftButtonDown = pMouse->bLeftDown;
				}
			}
		}
		// Since this function is called repeatedly during a drag we need to guard against displaying 
		// the message until the user has terminated the drag; i.e. released the mouse button.
		if (!bLeftButtonDown) {
			GUI_MsgBox("Events cannot be sized \nby moving the bottom of \nthe event outline box.");
		}
	}
	else if (dTop != mdEditBoxTop) {
		iRetVal = 0;
		BOOL bLeftButtonDown = TRUE;
		if (mpPlotPane != NULL) {
			PL_CTX *pPlCtx = mpPlotPane->GetPlotContext();
			if (pPlCtx != NULL) {
				PL_MOUSE *pMouse = pPlCtx->pMouse;
				if (pMouse != NULL) {
					bLeftButtonDown = pMouse->bLeftDown;
				}
			}
		}
		// Since this function is called repeatedly during a drag we need to guard against displaying 
		// the message until the user has terminated the drag; i.e. released the mouse button.
		if (!bLeftButtonDown) {
			GUI_MsgBox("Events cannot be sized \nby moving the top of the \nevent outline box.");
		}
	}
		
	if (iRetVal != 0) {
		mdLastBoxLeft = dLeft;
		mdLastBoxRight = dRight;
	}
							
	return(iRetVal);
}


// Determine max size event could be.  Constrained by 1)edges of display and 2)adjacent hilite regions.
void CEditEvents::DetermineEventLimits(UINT uiTraceIndex, double *pdMinNewEventX, double *pdMaxNewEventX)
{
	double dMinFullX, dMaxFullX, dMinVisibleX, dMaxVisibleX;

	if (mpPlotPane != NULL) {
		// Determine maximum range for new event.
		mpPlotPane->GetLimitsX(&dMinFullX, &dMaxFullX, &dMinVisibleX, &dMaxVisibleX);
		// Roger takes care of not allowing the user to drag the box border ouside of the visible
		// plot area. However, the user may zoom out after starting to create a new event or
		// editting boundaries of an ald event so the new bounds are the full data not just
		// the currently visible data.
		//*pdMinNewEventX = dMinVisibleX;
		//*pdMaxNewEventX = dMaxVisibleX;
		*pdMinNewEventX = dMinFullX;
		*pdMaxNewEventX = dMaxFullX;

		// Now find actual first and last data point.
		// Remember cursor's original position.
		double dCursorX, dCursorY;
		UINT uiAreaIndex;

		mpPlotPane->GetCursorPos(&dCursorX, &dCursorY, &uiAreaIndex);

		// snap the visible edges to the closest actual pts
		BOOL bOnNearestDataPt = TRUE;
		BOOL bUpdateDisplay = FALSE;
		// SetCursorPos for each edge with bOnNearestDataPt = TRUE then get actual cursor value...
		mpPlotPane->SetCursorPos(*pdMinNewEventX, uiTraceIndex, bOnNearestDataPt, bUpdateDisplay);
		mpPlotPane->GetCursorPos(pdMinNewEventX);
		mpPlotPane->SetCursorPos(*pdMaxNewEventX, uiTraceIndex, bOnNearestDataPt, bUpdateDisplay);
		mpPlotPane->GetCursorPos(pdMaxNewEventX);

		// Now set cursor back to original position.
		mpPlotPane->SetCursorPos(dCursorX, uiAreaIndex);

		// Now restrict even further if there is another hilite region before or after.
		// Must be at least one data point inbetween two highlight regions
		double dPrevLowX, dPrevHighX;
		BOOL bPrevRegion = mpPlotPane->PrevHiliteRegion(FALSE, &dPrevLowX, &dPrevHighX);
		if (bPrevRegion) {
			if (dPrevHighX > *pdMinNewEventX) {	// next to a hilite region, move 2 pts to the right of the hilite
				//*pdMinNewEventX = dPrevHighX + 2.0; // sfk change -1 to +2
				mpPlotPane->SetCursorPos(dPrevHighX, uiTraceIndex, bOnNearestDataPt, bUpdateDisplay); //sfk
				mpPlotPane->MoveCursor(2.0);	//sfk
				mpPlotPane->GetCursorPos(pdMinNewEventX);	//sfk
			}
		}

		// Now set cursor back to original position.
		mpPlotPane->SetCursorPos(dCursorX, uiAreaIndex);

		double dNextLowX, dNextHighX;
		BOOL bNextRegion = mpPlotPane->NextHiliteRegion(FALSE, &dNextLowX, &dNextHighX);
		if (bNextRegion) {
			if (dNextLowX < *pdMaxNewEventX) { // next to a hilite region, move 2 pts to left of hilite
				//*pdMaxNewEventX = dNextLowX - 2.0; // sfk change -1 to -2
				mpPlotPane->SetCursorPos(dNextLowX, uiTraceIndex, bOnNearestDataPt, bUpdateDisplay); //sfk
				mpPlotPane->MoveCursor(-2.0);	//sfk
				mpPlotPane->GetCursorPos(pdMaxNewEventX);	//sfk
			}
		}

		// Check to see are still in the visible region after moving left or right.  added by sfk
		if (*pdMinNewEventX < dMinFullX)
			*pdMinNewEventX = dMinFullX;
		if (*pdMaxNewEventX > dMaxFullX)
			*pdMaxNewEventX = dMaxFullX;

		// Now set cursor back to original position again.
		mpPlotPane->SetCursorPos(dCursorX, uiAreaIndex);  // sfk
	}
}
