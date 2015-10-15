
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