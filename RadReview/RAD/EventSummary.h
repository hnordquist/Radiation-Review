
/* EventSummary.h  Function Prototypes */

#ifndef EventSummary_h
	#define EventSummary_h

//#include "TextFile.h"

class CGUI_TextWin;

void EventDisplayByChan(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, CGUI_TextWin *pWin, BOOL bIncludeBkg);

void EventDisplayByTime(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, CGUI_TextWin *pWin, BOOL bGroup, BOOL bIncludeBkg);

void DailySummaryByChan(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, CGUI_TextWin *pWin, BOOL bIncludeBkg);

void DailySummaryByTime(DATE dStart, DATE dEnd, struct ANALYZE_PARMS *pAnalyzeChans, CGUI_TextWin *pWin, BOOL bIncludeBkg);

#endif
