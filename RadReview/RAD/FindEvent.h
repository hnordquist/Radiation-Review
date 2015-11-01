
/* FindEvent.H  Function Prototypes */

#ifndef FindEvent_h
	#define FindEvent_h

	//#include "TextFile.h"
	class CGUI_TextWin;
	class TextFile;

//	void PrintChanEventResults(db_event_rec *pdbEventRec, char *szChanName, unsigned long *pulNumEvents,
//					   unsigned long *pulTotalNum, CGUI_TextWin *pWin, TextFile *pFile,
//					   BOOL bTextWin, BOOL bLogFile, BOOL bDBFile);	// eventually delete
	void PrintChanEventResults(struct IDStruct ChanID, db_event_rec *pdbEventRec, char *szChanName, unsigned long *pulNumEvents,
					   unsigned long *pulTotalEvents, CGUI_TextWin *pWin, TextFile *pFile,
					   BOOL bTextWin, BOOL bLogFile, BOOL bDBFile);

//	int EventFind(DATE dStart, DATE dEnd, struct ANALYZE_CHANS *pAnalyzeChans, struct DATA_ANALYZE *pAnalyzeParms, BOOL bPrintAll, unsigned long *pulTotalEvents);	// eventually delete
	int EventFind(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, struct DATA_ANALYZE *pAnalyzeParms, BOOL bPrintAll, unsigned long *pulTotalEvents);
	void CloseTextDBFileForIR(const unsigned long ulTotalEvents);


#endif
