
/* FindMeas.H  Function Prototypes */

#ifndef FindMeas_H
	#define FindMeas_H

	void PrintMeasEventToTextWin(struct db_event_rec *pdbChanEvent, CGUI_TextWin *pWin, unsigned long *pulNumEvents);
//	int FindMeasurements(short sSta, DATE dStart, DATE dEnd, BOOL bDiagPrint, CGUI_TextWin *pWin);	// eventually delete
	int FindMeasurements(struct IDStruct StaID, DATE dStart, DATE dEnd, BOOL bDiagPrint, CGUI_TextWin *pWin);
	


#endif
