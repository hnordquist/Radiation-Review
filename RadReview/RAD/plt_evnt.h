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

#ifndef PLT_EVNT_H
	#define PLT_EVNT_H

enum EDIT_EVENTS_MODE {EEM_ADD_NEW, EEM_EDIT_PREV, EEM_DELETE_PREV, EEM_NONE};

class CEditEvents : public CGUI_CallBack {
	public:
		CEditEvents(CPlotDlg *pParentPlotDlg, CPlotPane *pPlotPane, CPlotDB *pPlotDB);
		~CEditEvents();
		SetFocus(BOOL bToEditEventsDlg = TRUE);
		void UpdateDlg();
		void SetFocusTo();
		void DisplayEmphasizedTrace(UINT uiTraceIndex, double dValLeft, double dValRight);
		int EventBoxEdgeMoveNotify(double dLeft, double dRight, double dTop, double dBottom);
		void ResetEventBoxHeight();

	protected:
		CPlotDlg *mpParentPlotDlg;
		CPlotPane *mpPlotPane;
		CPlotDB *mpPlotDB;
		CGUI_Dlg *mpEditEventDlg;

		PL_BOX *mpPlObjBox;
		double mdMinNewEventX;
		double mdMaxNewEventX;

		UINT muiPrevUpdateDlgCursorTraceIndex;
		double mdPrevUpdateDlgCursorLowValueRelSecs;
		double mdPrevUpdateDlgCursorHighValueRelSecs;

		EDIT_EVENTS_MODE meMode;
		BOOL SetMode(EDIT_EVENTS_MODE eMode);

		// Edit-event dlg fns
		void DetermineEventLimits(UINT uiTraceIndex, double *pdMinNewEventX, double *pdMaxNewEventX);
		BOOL CreateEventBox(UINT uiTraceIndex, double dValLeft, double dValRight);
		void StartEventEdit();
		BOOL DeletePlObjBox(double *pdBoxLeft, double *pdBoxRight, BOOL bCancel);
		void FinishEventEdit(BOOL bSaveChanges);
		void FinishCreateNewEvent(BOOL bSaveNewEvent);
		void UpdateDlgTimes(double dLowValueRelSecs = 0.0, double dHighValueRelSecs = 0.0);

		BOOL mbAnyEditChangesPending;
		BOOL ValidateEventIdChange(CGUI_Dlg *pDlg, RESOURCE_ID Id);
		BOOL ValidateEventStartTimeChange(CGUI_Dlg *pDlg, RESOURCE_ID Id);
		BOOL ValidateEventEndTimeChange(CGUI_Dlg *pDlg, RESOURCE_ID Id);
		void NotifyEventTypeChange(CGUI_Dlg *pDlg, RESOURCE_ID Id);

		void UpdateDlgTypePickList(UINT uiPL_ChanIndex);
		void UpdateDlgTypePickList(UINT uiPL_ChanIndex, double dLowValueRelSecs, double dHighValueRelSecs);
		void DeleteCurrentEvent(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		void EditCurrentEvent(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		void CreateDlg();
		void AlwaysOnTop(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		void CreateNewEvent(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		void GoToNextEvent(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		void GoToPrevEvent(CGUI_Dlg *pDlg, RESOURCE_ID CtrlID);
		void SetEditEventBtnsEnable();
		void SetEditEventEditFieldsEnable();

		BOOL EditEventDlgExitValidateFn(CGUI_Dlg *pDlg, GUI_ACTION ClosingAction);

		static COLORREF mEmphasizedColor;
		static float mfEmphasizedLineThicknessPts;
		BOOL mbEmphasizeActive;
		UINT muiEmphasizedTraceIndex;
		double mdActualMinEmphasizedX, mdActualMaxEmphasizedX;
		double mdEditBoxTop, mdEditBoxBottom;
		double mdLastBoxLeft, mdLastBoxRight;
};





#endif